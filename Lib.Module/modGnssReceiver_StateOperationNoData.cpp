#include "modGnssReceiver.h"

#include <utilsPacketNMEA.h>
#include <utilsPacketNMEAPayload.h>

#include <chrono>
#include <thread>

//#include <boost/signals2.hpp>

namespace mod
{

tGnssReceiver::tStateOperationNoData::tStateOperationNoData(tGnssReceiver* obj, const std::chrono::time_point<tClock>& startTime)
	:tState(obj), m_StartTime(startTime)
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateOperationNoData");
}

bool tGnssReceiver::tStateOperationNoData::operator()()
{
	if (!m_pObj->IsControlOperation())
	{
		ChangeState(new tStateStop(m_pObj, "operation"));
		return true;
	}

	if (m_pObj->IsReceivedData())
	{
		std::stringstream StrTime;
		m_pObj->SetStrTimePeriod(StrTime, m_StartTime);
		m_StartTime = tClock::now();

		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, StrTime.str());

		ChangeState(new tStateOperation(m_pObj, m_StartTime));
		return true;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	return true;
}

}
