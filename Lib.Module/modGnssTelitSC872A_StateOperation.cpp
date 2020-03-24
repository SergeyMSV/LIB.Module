#include "modGnssTelitSC872A.h"

#include <utilsPacketNMEA.h>
#include <utilsPacketNMEAPayload.h>

#include <chrono>
#include <thread>

//#include <boost/signals2.hpp>

namespace mod
{

tGnssTelitSC872A::tStateOperation::tStateOperation(tGnssTelitSC872A* obj, const std::string& value)
	:tState(obj)
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateOperation: %s", value.c_str());
}

template<class T, class U>
void SetParam(T& valDst, U valSrc, bool& check)
{
	if (valSrc.Absent)
	{
		check = false;
	}
	else
	{
		valDst = valSrc.Value;
	}
}

void tGnssTelitSC872A::tStateOperation::operator()()
{
	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "PREVED");

	utils::tVectorUInt8 Data;

	while (true)
	{
		if (m_pObj->IsReceivedData())
		{
			utils::tVectorUInt8 DataChunk = m_pObj->GetReceivedDataChunk();

			Data.insert(Data.end(), DataChunk.cbegin(), DataChunk.cend());//C++14

			while (true)
			{
				tPacketNMEA Packet;

				if (tPacketNMEA::Find(Data, Packet))
				{
					utils::packet_NMEA::tPayloadCommon::value_type PacketData = Packet.GetPayload();

					if (tMsgGSV::Try(PacketData))
					{
						tMsgGSV Msg(PacketData);
						m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightMagenta, PacketData[0] + " " + Msg.MsgQty.ToString() + " " + Msg.MsgNum.ToString() + " " + Msg.SatelliteQty.ToString());

						for (auto& i : Msg.Satellite)//C++11
						{
							m_DataSet.Satellite.push(std::forward<tGNSS_Satellite>(i));
						}
					}
					else if (tMsgRMC::Try(PacketData))
					{
						tMsgRMC Msg(PacketData);
						m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightMagenta, PacketData[0] + " " + Msg.Date.ToString() + " " + Msg.Time.ToString());

						if (!Msg.Date.Absent)
						{
							m_DataSet.Year = Msg.Date.Year;
							m_DataSet.Month = Msg.Date.Month;
							m_DataSet.Day = Msg.Date.Day;
						}

						if (!Msg.Time.Absent)
						{
							m_DataSet.Hour = Msg.Time.Hour;
							m_DataSet.Minute = Msg.Time.Minute;
							m_DataSet.Second = Msg.Time.Second;
						}

						m_DataSet.Check_DateTime = !Msg.Date.Absent && !Msg.Time.Absent;

						SetParam(m_DataSet.Valid, Msg.Valid, m_DataSet.Check_GNSS);
						SetParam(m_DataSet.Latitude, Msg.Latitude, m_DataSet.Check_GNSS);
						SetParam(m_DataSet.Longitude, Msg.Longitude, m_DataSet.Check_GNSS);
						SetParam(m_DataSet.Speed, Msg.Speed, m_DataSet.Check_GNSS);
						SetParam(m_DataSet.Course, Msg.Course, m_DataSet.Check_GNSS);

						m_pObj->OnChanged(m_DataSet);//TEST
					}
					else
					{
						m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Yellow, PacketData[0]);

						while (!m_DataSet.Satellite.empty())
						{
							m_DataSet.Satellite.pop();//TEST
						}
					}
				}
				else
				{
					break;
				}
			}

			if (Data.size() > 1024)//[#]
			{
				ChangeState(new tStateError(m_pObj, "buffer overrun"));
				return;
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			//ChangeState(new tStateOperation(m_pObj, "transaction"));
			//return;
		}
	}
}

void tGnssTelitSC872A::tStateOperation::operator()(const utils::tVectorUInt8& data)
{
	m_pObj->m_pLog->WriteHex(false, utils::tLogColour::LightYellow, "DATA", data);
}

}

