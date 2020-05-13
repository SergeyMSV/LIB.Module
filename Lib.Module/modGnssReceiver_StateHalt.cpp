#include "modGnssReceiver.h"

namespace mod
{

tGnssReceiver::tStateHalt::tStateHalt(tGnssReceiver* obj, const std::string& value)
	:tState(obj)
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateHalt: %s", value.c_str());

	if (m_pObj->IsControlRestart())
	{
		m_pObj->m_Control_Restart = false;
	}
}

void tGnssReceiver::tStateHalt::Go()
//bool tGnssReceiver::tStateHalt::operator()()
{
	if (!m_pObj->m_Control_Exit)
	{
		if (m_pObj->IsControlOperation())
		{
			ChangeState(new tStateStart(m_pObj, "start...s"));
			return;// true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		//return true;
	}

	//return false;
}

}
