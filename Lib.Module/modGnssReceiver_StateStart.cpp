#include "modGnssReceiver.h"

//#include <chrono>
//#include <thread>

namespace mod
{

tGnssReceiver::tStateStart::tStateStart(tGnssReceiver* obj, const std::string& value)
	:tState(obj, "StateStart")
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateStart: %s", value.c_str());

	if (m_pObj->IsControlRestart())
	{
		m_pObj->m_Control_Restart = false;
	}

	//m_pObj->ClearReceivedData();
}


void tGnssReceiver::tStateStart::OnTaskScriptDone()
{
	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnTaskScriptDone");

	ChangeState(new tStateStart(m_pObj, "start"));
	return;
}

void tGnssReceiver::tStateStart::OnTaskScriptFailed()
{
	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnTaskScriptFailed");

	ChangeState(new tStateStart(m_pObj, "start"));
	return;
}

void tGnssReceiver::tStateStart::OnTaskScriptFailed(const std::string& msg)
{
	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnTaskScriptFailed: " + msg);

	ChangeState(new tStateStart(m_pObj, "start"));
	return;
}

void tGnssReceiver::tStateStart::Go()
{

}

void tGnssReceiver::tStateStart::OnReceived(const tPacketNMEA_Template& value)
{
	std::string Payload = value.GetPayload();

	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnReceived: " + Payload);

	//if ()

	//if (m_TaskScriptSentMsg && !m_TaskScript.empty() && value.GetPayload().find(m_TaskScript.front().RspHead) == 0)
	//{
	//	////
	//	{//[TEST]
	//		auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_StartTime).count();//C++11
	//		std::stringstream StrTime;
	//		StrTime << value.GetPayload() << " --- " << Time_us << " us";
	//		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, StrTime.str());
	//	}
	//	////

	//	m_TaskScriptTime_us = m_TaskScript.front().TimePause_us;

	//	m_TaskScriptSentMsg = false;

	//	if (value.GetPayload() == m_TaskScript.front().RspHead + m_TaskScript.front().RspBody ||
	//		m_TaskScript.front().CaseRspWrong.empty())
	//	{
	//		m_TaskScript.pop_front();
	//	}
	//	else if(m_CaseRspWrongLast != m_TaskScript.front().CaseRspWrong)
	//	{
	//		m_CaseRspWrongLast = m_TaskScript.front().CaseRspWrong;

	//		tGnssTaskScript Script = m_pObj->GetTaskScript(m_CaseRspWrongLast);

	//		m_TaskScript.insert(m_TaskScript.begin(), Script.begin(), Script.end());
	//	}
	//	else
	//	{
	//		//ERROR!!
	//		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, "ERROR!!!! OnReceived");
	//		return;
	//	}
	//}
}

}