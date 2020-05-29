#include "modGnssReceiver.h"

#include <chrono>
#include <thread>

namespace mod
{

tGnssReceiver::tState::tState(tGnssReceiver* obj)
	:m_pObj(obj)
{
	m_pObj->ClearReceivedData();
}

tGnssReceiver::tState::tState(tGnssReceiver* obj, const std::string& taskScriptID)
	:tState(obj)
{
	m_TaskScript = m_pObj->GetTaskScript(taskScriptID);
}

tGnssReceiver::tState::~tState()
{
	delete m_pCmd;
}

bool tGnssReceiver::tState::operator()()
{
	if (m_pCmd && (*m_pCmd)())
		return true;

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
			if (m_pCmd && m_pCmd->OnReceived(Packet))//ChangeState
				return true;

			OnReceived(Packet);//ChangeState
			return true;
		}
	}

	TaskScript();

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	Go();//ChangeState
	return true;
}

bool tGnssReceiver::tState::Halt()
{
	ChangeState(new tStateStop(m_pObj, "halt"));
	return true;
}

bool tGnssReceiver::tState::SetTaskScript(const std::string& taskScriptID)
{
	tGnssTaskScript Script = m_pObj->GetTaskScript(taskScriptID);

	for (auto& i : Script)
	{
		m_TaskScript.push_back(std::move(i));
	}

	return true;
}

void tGnssReceiver::tState::TaskScript()
{
	if (m_pCmd == nullptr && !m_TaskScript.empty())
	{
		tGnssTaskScriptCmd* Ptr = m_TaskScript.front().get();

		if (Ptr != nullptr)
		{
			switch (Ptr->GetID())
			{
			case tGnssTaskScriptCmd::tID::GPI:
			{
				m_pCmd = new tCmdGPI(this, std::move(m_TaskScript.front()));
				break;
			}
			case tGnssTaskScriptCmd::tID::GPO:
			{
				m_pCmd = new tCmdGPO(this, std::move(m_TaskScript.front()));
				break;
			}
			case tGnssTaskScriptCmd::tID::REQ:
			{
				m_pCmd = new tCmdREQ(this, std::move(m_TaskScript.front()));
				break;
			}
			default://ERROR
			{
				m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightRed, "ERR: unknown task");

				//[TBD] throw an exception or do nothing

				break;
			}
			}

			m_TaskScript.pop_front();
		}
	}
}



//bool tGnssReceiver::tState::TaskScript_Handle(tGnssTaskScriptCmdGPI* ptr)
//{
//	m_TaskScript.pop_front();//[TBD] Wait for appearance of GPI
//
//	return false;
//}
//
//bool tGnssReceiver::tState::TaskScript_Handle(tGnssTaskScriptCmdGPO* ptr)
//{
//	if (ptr->ID == "PWR")
//	{
//		m_TaskScriptActive = true;
//
//		m_pObj->Board_PowerSupply(ptr->State);
//	}
//	else if (ptr->ID == "RST")
//	{
//		m_TaskScriptActive = true;
//
//		m_pObj->Board_Reset(ptr->State);
//	}
//	else
//	{
//		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightRed, "ERR: unknown GPO");
//
//		//[TBD] throw an exception or do nothing
//	}
//
//	m_TaskScript.pop_front();
//
//	return true;
//}

//bool tGnssReceiver::tState::TaskScript_OnReceived(const tPacketNMEA_Template& value)
//{
//	if (!m_TaskScript.empty() && m_TaskScript.front().get()->GetID() == tGnssTaskScriptCmd::tID::REQ && m_TaskScriptActive)
//	{
//		tGnssTaskScriptCmdREQ* Ptr = static_cast<tGnssTaskScriptCmdREQ*>(m_TaskScript.front().get());
//
//		if (value.GetPayload().find(Ptr->RspHead) == 0)
//		{
//			////
//			{//[TEST]
//				auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_TaskScriptStartTime).count();//C++11
//				std::stringstream StrTime;
//				StrTime << value.GetPayload() << " --- " << Time_us << " us";
//				m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, StrTime.str());
//			}
//			////
//
//			m_TaskScriptTime_us = Ptr->TimePause_us;
//
//			m_TaskScriptActive = false;
//
//			if (value.GetPayload() == Ptr->RspHead + Ptr->RspBody || Ptr->CaseRspWrong.empty())
//			{
//				m_TaskScript.pop_front();
//				return true;
//			}
//			else if (m_TaskScriptCaseRspWrongLast != Ptr->CaseRspWrong)
//			{
//				m_TaskScriptCaseRspWrongLast = Ptr->CaseRspWrong;
//
//				tGnssTaskScript Script = m_pObj->GetTaskScript(m_TaskScriptCaseRspWrongLast);
//
//				for (tGnssTaskScript::reverse_iterator i = Script.rbegin(); i != Script.rend(); ++i)//C++11
//				{
//					m_TaskScript.push_front(std::move(*i));
//				}
//			}
//			else
//			{
//				OnTaskScriptFailed(m_TaskScriptCaseRspWrongLast);
//				return true;
//			}
//
//			return true;
//		}
//	}
//
//	return false;
//}

//bool tGnssReceiver::tState::TaskScript_Handle(tGnssTaskScriptCmdREQ* ptr)
//{
//	//if (m_pCmd == nullptr)
//	//{
//	//	m_pCmd = new tCmdREQ(this, Queue.front());
//	//}
//	/*if (!m_TaskScriptActive)
//	{
//		m_TaskScriptActive = true;
//
//		m_TaskScriptStartTime = tClock::now();
//
//		std::string Msg = ptr->Msg;
//
//		if (!Msg.empty())
//		{
//			tPacketNMEA_Template Packet(Msg);
//
//			m_pObj->Board_Send(Packet.ToVector());
//		}
//
//		m_TaskScriptTime_us = ptr->RspWait_us;
//	}
//	else if (m_TaskScriptTime_us > 0)
//	{
//		auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_TaskScriptStartTime).count();//C++11
//
//		if (m_TaskScriptTime_us < Time_us)
//		{
//			OnTaskScriptFailed();
//			return true;
//		}
//	}*/
//
//	return false;
//}

bool tGnssReceiver::tState::OnCmdDone()
{
	if (m_TaskScript.empty())
	{
		OnTaskScriptDone();
		return true;
	}

	ResetCmd();
	return true;
}

bool tGnssReceiver::tState::OnCmdFailed()
{
	return false;
}

void tGnssReceiver::tState::OnCmdTaskScript(std::unique_ptr<tGnssTaskScriptCmd> cmd, const std::string& taskScriptID)
{
	m_TaskScript.push_front(std::move(cmd));

	if (m_OnCmdTaskScriptIDLast != taskScriptID)
	{
		m_OnCmdTaskScriptIDLast = taskScriptID;

		tGnssTaskScript Script = m_pObj->GetTaskScript(taskScriptID);

		for (tGnssTaskScript::reverse_iterator i = Script.rbegin(); i != Script.rend(); ++i)//C++11
		{
			m_TaskScript.push_front(std::move(*i));
		}
	}
	else
	{
		OnTaskScriptFailed(m_OnCmdTaskScriptIDLast);//ChangeState
		return;
	}
}

void tGnssReceiver::tState::ResetCmd()
{
	delete m_pCmd;
	m_pCmd = nullptr;
}

}
