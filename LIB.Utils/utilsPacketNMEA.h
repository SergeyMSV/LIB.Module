///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEA.h
//
// Standard ISO/IEC 114882, C++14
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2019 01 31  |
// |      2     |   2019 02 07  | Added tPacket(std::string& address, int payloadItemQty, bool encapsulation = false);
// |      3     |   2019 05 01  | Refactored
// |      4     |   2019 09 20  | Refactored
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsCRC.h"
#include "utilsPacket.h"

#include <string>
#include <vector>

namespace utils
{
	namespace packet_NMEA
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
template <class TPayload, unsigned char stx = '$'>
struct tFormat
{
	enum { STX = stx, CTX = '*' };

protected:
	template <class tMsg>
	void SetPayloadIDs(const tMsg& msg) { }

	static tVectorUInt8 TestPacket(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		std::size_t Size = std::distance(cbegin, cend);

		if (Size >= GetSize(0) && *cbegin == STX)
		{
			tVectorUInt8::const_iterator Begin = cbegin + 1;
			tVectorUInt8::const_iterator End = std::find(Begin, cend, CTX);

			if (End != cend)
			{
				std::size_t DataSize = std::distance(Begin, End);

				if (Size >= GetSize(DataSize) && VerifyCRC(Begin, DataSize))
				{
					return tVectorUInt8(cbegin, cbegin + GetSize(DataSize));
				}
			}
		}

		return tVectorUInt8();
	}

	static bool TryParse(const tVectorUInt8& packetVector, tFormat& format, TPayload& payload)
	{
		if (packetVector.size() >= GetSize(0) && packetVector[0] == STX)
		{
			tVectorUInt8::const_iterator Begin = packetVector.cbegin() + 1;
			tVectorUInt8::const_iterator End = std::find(Begin, packetVector.cend(), CTX);

			if (End != packetVector.cend())
			{
				std::size_t DataSize = std::distance(Begin, End);

				if (packetVector.size() == GetSize(DataSize) && VerifyCRC(Begin, DataSize))
				{
					payload = TPayload(Begin, End);

					return true;
				}
			}
		}

		return false;
	}

	static std::size_t GetSize(std::size_t payloadSize) { return payloadSize + 6; };//$*xx\xd\xa

	void Append(tVectorUInt8& dst, const TPayload& payload) const
	{
		dst.push_back(STX);

		payload.Append(dst);

		unsigned char CRC = utils::crc::CRC08_NMEA<tVectorUInt8::const_iterator>(dst.cbegin() + 1, dst.cend());

		dst.push_back(CTX);

		char StrCRC[5];
		std::sprintf(StrCRC, "%02X", CRC);

		dst.push_back(StrCRC[0]);
		dst.push_back(StrCRC[1]);

		dst.push_back(0x0D);
		dst.push_back(0x0A);
	}

private:
	static bool VerifyCRC(tVectorUInt8::const_iterator begin, std::size_t crcDataSize)
	{
		auto CRC = utils::crc::CRC08_NMEA(begin, begin + crcDataSize);

		tVectorUInt8::const_iterator CRCBegin = begin + crcDataSize + 1;//1 for '*'

		auto CRCReceived = utils::Read<unsigned char>(CRCBegin, CRCBegin + 2, utils::tRadix_16);

		return CRC == CRCReceived;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <class TPayload> struct tFormatNMEA : public tFormat<TPayload, '$'> { };
template <class TPayload> struct tFormatNMEABin : public tFormat<TPayload, '!'> { };
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadCommon
{
	typedef std::vector<std::string> value_type;

	value_type Data;

	tPayloadCommon() { }

	tPayloadCommon(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		std::string LocalString;

		for (tVectorUInt8::const_iterator i = cbegin; i != cend; ++i)
		{
			if (*i == ',')
			{
				Data.push_back(LocalString);
				LocalString.clear();
			}
			else
			{
				LocalString.push_back(static_cast<char>(*i));
			}
		}

		Data.push_back(LocalString);
	}

	std::size_t GetSize() const
	{
		std::size_t Size = 0;

		for (std::size_t i = 0; i < Data.size(); ++i)
		{
			Size += Data[i].size();

			if (i != Data.size() - 1)
			{
				Size += 1;
			}
		}

		return Size;
	}

	void Append(tVectorUInt8& dst) const
	{
		for (std::size_t i = 0; i < Data.size(); ++i)
		{
			dst.insert(dst.end(), Data[i].cbegin(), Data[i].cend());

			if (i != Data.size() - 1)
			{
				dst.push_back(',');
			}
		}
	}
};

	}
}
