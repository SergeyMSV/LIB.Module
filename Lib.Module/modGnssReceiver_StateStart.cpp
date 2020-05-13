#include "modGnssReceiver.h"

#include <chrono>
#include <thread>

namespace mod
{

tGnssReceiver::tStateStart::tStateStart(tGnssReceiver* obj, const std::string& value)
	:tState(obj)
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateStart: %s", value.c_str());

	if (m_pObj->IsControlRestart())
	{
		m_pObj->m_Control_Restart = false;
	}

	m_pObj->ClearReceivedData();

	m_TaskScript = m_pObj->GetTaskScript("StateStart");
}

void tGnssReceiver::tStateStart::Go()
{
	auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_StartTime).count();//C++11

	if (Time_us < m_TaskScriptTime_us)
		return;

	if (!m_TaskScript.empty())
	{
		if (!m_TaskScriptSentMsg)
		{
			m_TaskScriptSentMsg = !m_TaskScript.front().RspHead.empty();

			m_StartTime = tClock::now();

			std::string Msg = m_TaskScript.front().Msg;

			if (!Msg.empty())
			{
				tPacketNMEA_Template Packet(Msg);

				m_pObj->Board_Send(Packet.ToVector());
			}

			m_TaskScriptTime_us = m_TaskScript.front().RspWait_us;
		}
		else
		{
			//auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_StartTime).count();//C++11

			if (m_TaskScriptTime_us > 0 && m_TaskScriptTime_us < Time_us)
			{
				//ERROR - time out
				m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "TimeERROR");
			}
			//else
			//{
			//	//m_TaskScriptSentMsg = false;
			//	//m_TaskScript.pop();
			//	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "TimeHREN");
			//}
		}
	}
	else if(Time_us > m_TaskScriptTime_us)
	{
		//FINISH --- NEXT STATE!!!
		ChangeState(new tStateStart(m_pObj, "start"));
		return;
	}
}

void tGnssReceiver::tStateStart::OnReceived(const tPacketNMEA_Template& value)
{
	if (m_TaskScriptSentMsg && !m_TaskScript.empty() && value.GetPayload().find(m_TaskScript.front().RspHead) == 0)
	{
		////
		{//[TEST]
			auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_StartTime).count();//C++11
			std::stringstream StrTime;
			StrTime << value.GetPayload() << " --- " << Time_us << " us";
			m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, StrTime.str());
		}
		////

		m_TaskScriptTime_us = m_TaskScript.front().TimePause_us;

		m_TaskScriptSentMsg = false;

		if (value.GetPayload() == m_TaskScript.front().RspHead + m_TaskScript.front().RspBody ||
			m_TaskScript.front().CaseRspWrong.empty())
		{
			m_TaskScript.pop_front();
		}
		else if(m_CaseRspWrongLast != m_TaskScript.front().CaseRspWrong)
		{
			m_CaseRspWrongLast = m_TaskScript.front().CaseRspWrong;

			tGnssTaskScript Script = m_pObj->GetTaskScript(m_CaseRspWrongLast);

			m_TaskScript.insert(m_TaskScript.begin(), Script.begin(), Script.end());
		}
		else
		{
			//ERROR!!
			m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, "ERROR!!!! OnReceived");
			return;
		}
	}
}

}