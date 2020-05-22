//#include "modGnssReceiver.h"
//
//namespace mod
//{
//
//tGnssReceiver::tStateError::tStateError(tGnssReceiver* obj, const std::string& value)
//	:tState(obj)
//{
//	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateError: %s", value.c_str());
//
//	if (m_pObj->IsControlRestart())
//	{
//		m_pObj->m_Control_Restart = false;
//	}
//
//	m_pObj->ClearReceivedData();
//
//	m_TaskScript = m_pObj->GetTaskScript("HandleError");
//}
//
//void tGnssReceiver::tStateError::Go()
//{
//	if (!m_TaskScript.empty())
//	{
//		if (!m_TaskScriptActive)
//		{
//			m_TaskScriptActive = true;
//
//			m_StartTime = tClock::now();
//
//			tPacketNMEA_Template Packet(m_TaskScript.front().Msg);
//
//			m_pObj->Board_Send(Packet.ToVector());
//
//			m_TaskScriptTime_us = m_TaskScript.front().RspWait_us;
//		}
//		else
//		{
//			auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_StartTime).count();//C++11
//
//			if (Time_us > m_TaskScriptTime_us)
//			{
//				//ERROR - time out
//				m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "TimeERROR");
//			}
//			//else
//			//{
//			//	m_TaskScriptActive = false;
//			//	m_TaskScript.pop();
//			//}
//		}
//	}
//}
//
//void tGnssReceiver::tStateError::OnReceived(const tPacketNMEA_Template& value)
//{
//	if (m_TaskScriptActive && !m_TaskScript.empty() && value.GetPayload() == m_TaskScript.front().RspHead)//[TBD]
//	{
//		auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_StartTime).count();//C++11
//
//		m_TaskScriptTime_us = m_TaskScript.front().TimePause_us;
//
//		m_TaskScriptActive = false;
//		m_TaskScript.pop_front();
//
//
//		std::stringstream StrTime;
//		StrTime << value.GetPayload() << " --- " << Time_us << " us";
//
//		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, StrTime.str());
//	}
//}
//
//
//
///*bool tGnssReceiver::tStateError::operator()()
//{
//	if (++m_Counter < 10)
//	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(10));
//	}
//	else
//	{
//		m_pObj->m_pLog->WriteLine();
//
//		ChangeState(new tStateStop(m_pObj, "lalala"));
//		return true;
//	}
//
//	m_pObj->m_pLog->Write(false, utils::tLogColour::LightRed, "e");
//
//	return true;
//}*/
//
//}
