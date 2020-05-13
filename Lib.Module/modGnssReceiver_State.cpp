#include "modGnssReceiver.h"

namespace mod
{

tGnssReceiver::tState::tState(tGnssReceiver* obj)
	:m_pObj(obj)
{

}

bool tGnssReceiver::tState::operator()()
{
	if (m_pObj->IsReceivedData())
	{
		//	if (m_NoData)
		//	{
		//		m_NoData = false;

		//		std::stringstream StrTime;
		//		m_pObj->SetStrTimePeriod(StrTime, m_StartTime);
		//		m_StartTime = tClock::now();

		//		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, StrTime.str());
		//	}

		utils::tVectorUInt8 DataChunk = m_pObj->GetReceivedDataChunk();

		m_ReceivedData.insert(m_ReceivedData.end(), DataChunk.cbegin(), DataChunk.cend());//C++14

	//	while (m_pObj->IsControlOperation())
	//	{

		tPacketNMEA_Template Packet;

		std::size_t PacketSize = tPacketNMEA_Template::Find(m_ReceivedData, Packet);

		if (PacketSize)
		{
			OnReceived(Packet);
		}
	}

	Go();//ChangeState
	return true;
}

bool tGnssReceiver::tState::Halt()
{
	ChangeState(new tStateStop(m_pObj, "halt"));
	return true;
}

}
