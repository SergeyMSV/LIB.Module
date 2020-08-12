#include "modGnssReceiver.h"

namespace mod
{

tGnssReceiver::tStateHalt::tStateHalt(tGnssReceiver* obj, const std::string& value)
	:tState(obj)
{
	std::stringstream SStr;
	SStr << "tStateHalt: " << value;
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, SStr.str());

	if (m_pObj->IsControlRestart())
	{
		m_pObj->m_Control_Restart = false;
	}
}

bool tGnssReceiver::tStateHalt::Go()
{
	if (m_pObj->m_Control_Exit)
		return false;

	if (m_pObj->IsControlOperation())
	{
		ChangeState(new tStateStart(m_pObj, "start...s"));
		return true;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	return true;
}

}
