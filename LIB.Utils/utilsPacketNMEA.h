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
// |      5     |   2020 09 06  | Corrected tFormat::Append(tVectorUInt8& dst, const TPayload& payload) const
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
	enum : unsigned char { STX = stx, CTX = '*' };

protected:
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

	static bool TryParse(const tVectorUInt8& packetVector, TPayload& payload)
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

	static void Append(tVectorUInt8& dst, const TPayload& payload)
	{
		dst.reserve(GetSize(payload.size()));

		dst.push_back(STX);

		for (auto i : payload)
		{
			dst.push_back(i);
		}

		unsigned char CRC = utils::crc::CRC08_NMEA(payload.begin(), payload.end());

		dst.push_back(CTX);

		char StrCRC[10]{};
		std::sprintf(StrCRC, "%02X\xd\xa", CRC);

		dst.insert(dst.end(), StrCRC, StrCRC + 4);
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

	class tIterator
	{
		friend struct tPayloadCommon;

		const tPayloadCommon* m_pObj = nullptr;

		std::size_t m_DataSize = 0;
		std::size_t m_DataIndex = 0;
		const char* m_DataPtr = nullptr;

		tIterator() = delete;
		tIterator(const tPayloadCommon* obj, bool begin) :m_pObj(obj), m_DataSize(m_pObj->size())
		{
			if (m_DataSize > 0 && m_pObj->Data[0].size() > 0)
			{
				if (begin)
				{
					m_DataPtr = &m_pObj->Data[0][0];
				}
				else
				{
					m_DataIndex = m_DataSize;
				}
			}
		}

	public:
		tIterator& operator ++ ()
		{
			if (m_DataIndex < m_DataSize)
			{
				++m_DataIndex;
			}

			std::size_t DataIndex = m_DataIndex;

			for (const std::string& i : m_pObj->Data)
			{
				std::size_t StrSize = i.size();

				if (DataIndex >= StrSize + 1)
				{
					DataIndex -= StrSize + 1;
				}
				else if (DataIndex == StrSize)
				{
					static char Separator = ',';
					m_DataPtr = &Separator;
					break;
				}
				else
				{
					m_DataPtr = &i[DataIndex];
					break;
				}
			}

			return *this;
		}

		bool operator != (const tIterator& val) const
		{
			return m_DataIndex != val.m_DataIndex;
		}

		const char operator * () const
		{
			assert(m_DataPtr != nullptr);

			return *m_DataPtr;
		}
	};

	typedef tIterator iterator;

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

	std::size_t size() const
	{
		std::size_t Size = 0;

		for (std::size_t i = 0; i < Data.size(); ++i)
		{
			Size += Data[i].size();
		}

		if (Data.size() > 0)
		{
			Size += Data.size() - 1;
		}

		return Size;
	}

	iterator begin() const
	{
		return iterator(this, true);
	}

	iterator end() const
	{
		return iterator(this, false);
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadString
{
	typedef std::string value_type;
	typedef value_type::const_iterator iterator;

	value_type Data;

	tPayloadString() { }

	tPayloadString(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		Data.insert(Data.end(), cbegin, cend);
	}

	std::size_t size() const
	{
		return Data.size();
	}

	iterator begin() const
	{
		return Data.begin();
	}

	iterator end() const
	{
		return Data.end();
	}
};

	}
}
