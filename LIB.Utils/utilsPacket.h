///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacket.h
//
// Standard ISO/IEC 114882, C++14
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2019 06 20  |
// |      2     |   2020 03 26  | std::size_t Find(tVectorUInt8& receivedData, tPacket& packet) instead of bool Find(tVectorUInt8& receivedData, tPacket& packet)
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsBase.h"

#include <algorithm>

namespace utils
{
	namespace packet
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
template
<
	template <class> class TFormat,
	class TPayload
>
class tPacket : public TFormat<TPayload>, public TPayload
{
public:
	typedef typename TPayload::value_type payload_value_type;

	tPacket() { }
	explicit tPacket(const payload_value_type& value)
	{
		TPayload::Data = value;
	}

	static std::size_t Find(tVectorUInt8& receivedData, tPacket& packet)
	{
		tVectorUInt8::const_iterator Begin = receivedData.cbegin();

		for (;;)
		{
			Begin = std::find(Begin, receivedData.cend(), TFormat<TPayload>::STX);

			if (Begin == receivedData.cend())
			{
				break;
			}

			tVectorUInt8 PacketVector = TFormat<TPayload>::TestPacket(Begin, receivedData.cend());

			if (PacketVector.size() > 0)
			{
				if (TryParse(PacketVector, packet))
				{
					std::size_t EraseSize = std::distance(receivedData.cbegin(), Begin);

					EraseSize += PacketVector.size();

					receivedData.erase(receivedData.begin(), receivedData.begin() + EraseSize);

					return PacketVector.size();
				}
			}

			Begin++;
		}

		return 0;
	}

	static bool TryParse(const tVectorUInt8& packetVector, tPacket& packet)
	{
		return TFormat<TPayload>::TryParse(packetVector, packet, packet);
	}

	payload_value_type GetPayload() const
	{
		return TPayload::Data;
	}

	void SetPayload(const payload_value_type& value)
	{
		TPayload::Data = value;
	}

	template <class T>
	void SetMsg(const T& msg)
	{
		TFormat<TPayload>::SetPayloadIDs(msg);

		TPayload::Data = msg.ToVector();
	}

	tVectorUInt8 ToVector()
	{
		std::size_t PayloadSize = TPayload::GetSize();

		std::size_t PacketSize = TFormat<TPayload>::GetSize(PayloadSize);

		tVectorUInt8 PacketVector;

		PacketVector.reserve(PacketSize);

		TFormat<TPayload>::Append(PacketVector, *this);

		return PacketVector;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadCommon
{
	typedef tVectorUInt8 value_type;

	value_type Data;

	tPayloadCommon() { }

	tPayloadCommon(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		Data.insert(Data.end(), cbegin, cend);
	}

	std::size_t GetSize() const { return Data.size(); }

	void Append(tVectorUInt8& dst) const
	{
		dst.insert(dst.end(), Data.cbegin(), Data.cend());
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
	}
}