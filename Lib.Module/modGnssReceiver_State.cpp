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
			if (TaskScript_OnReceived(Packet))//ChangeState
				return true;

			OnReceived(Packet);
		}
	}

	if (TaskScript())//ChangeState
		return true;

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

bool tGnssReceiver::tState::TaskScript()
{
	auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_TaskScriptStartTime).count();//C++11

	if (Time_us < m_TaskScriptTime_us)
		return false;

	if (!m_TaskScript.empty())
	{
		tGnssTaskScriptBase* Ptr = m_TaskScript.front().get();

		if (Ptr != nullptr)
		{
			switch (Ptr->GetID())
			{
			case tGnssTaskScriptBase::tID::Cmd:
			{
				if (TaskScript_Handle(static_cast<tGnssTaskScriptCmd*>(Ptr)))
					return true;
				break;
			}
			case tGnssTaskScriptBase::tID::GPI:
			{
				if (TaskScript_Handle(static_cast<tGnssTaskScriptGPI*>(Ptr)))
					return true;
				break;
			}
			case tGnssTaskScriptBase::tID::GPO:
			{
				if (TaskScript_Handle(static_cast<tGnssTaskScriptGPO*>(Ptr)))
					return true;
				break;
			}
			default://ERROR
			{
				m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightRed, "ERR: unknown task");

				//[TBD] throw an exception or do nothing

				m_TaskScript.pop_front();

				break;
			}
			}
		}
	}
	else if (m_TaskScriptTime_us > 0 && m_TaskScriptTime_us < Time_us)
	{
		m_TaskScriptTime_us = 0;

		OnTaskScriptDone();
		return true;
	}

	return false;
}

bool tGnssReceiver::tState::TaskScript_Handle(tGnssTaskScriptCmd* ptr)
{
	if (!m_TaskScriptSentMsg)
	{
		m_TaskScriptSentMsg = !ptr->RspHead.empty();

		m_TaskScriptStartTime = tClock::now();

		std::string Msg = ptr->Msg;

		if (!Msg.empty())
		{
			tPacketNMEA_Template Packet(Msg);

			m_pObj->Board_Send(Packet.ToVector());
		}

		m_TaskScriptTime_us = ptr->RspWait_us;
	}
	else if (m_TaskScriptTime_us > 0)
	{
		auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_TaskScriptStartTime).count();//C++11

		if (m_TaskScriptTime_us < Time_us)
		{
			OnTaskScriptFailed();
			return true;
		}
	}

	return false;
}

bool tGnssReceiver::tState::TaskScript_Handle(tGnssTaskScriptGPI* ptr)
{
	m_TaskScript.pop_front();//[TBD] Wait for appearance of GPI

	return false;
}

bool tGnssReceiver::tState::TaskScript_Handle(tGnssTaskScriptGPO* ptr)
{
	if (ptr->ID == "PWR")
	{
		m_pObj->Board_PowerSupply(ptr->State);
	}
	else if (ptr->ID == "RST")
	{
		m_pObj->Board_Reset(ptr->State);
	}
	else
	{
		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightRed, "ERR: unknown GPO");

		//[TBD] throw an exception or do nothing
	}

	m_TaskScript.pop_front();

	return true;
}

bool tGnssReceiver::tState::TaskScript_OnReceived(const tPacketNMEA_Template& value)
{
	if (!m_TaskScript.empty() && m_TaskScript.front().get()->GetID() == tGnssTaskScriptBase::tID::Cmd && m_TaskScriptSentMsg)
	{
		tGnssTaskScriptCmd* Ptr = static_cast<tGnssTaskScriptCmd*>(m_TaskScript.front().get());

		if (value.GetPayload().find(Ptr->RspHead) == 0)
		{
			////
			{//[TEST]
				auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - m_TaskScriptStartTime).count();//C++11
				std::stringstream StrTime;
				StrTime << value.GetPayload() << " --- " << Time_us << " us";
				m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, StrTime.str());
			}
			////

			m_TaskScriptTime_us = Ptr->TimePause_us;

			m_TaskScriptSentMsg = false;

			if (value.GetPayload() == Ptr->RspHead + Ptr->RspBody || Ptr->CaseRspWrong.empty())
			{
				m_TaskScript.pop_front();
				return true;
			}
			else if (m_TaskScriptCaseRspWrongLast != Ptr->CaseRspWrong)
			{
				m_TaskScriptCaseRspWrongLast = Ptr->CaseRspWrong;

				tGnssTaskScript Script = m_pObj->GetTaskScript(m_TaskScriptCaseRspWrongLast);

				for (tGnssTaskScript::reverse_iterator i = Script.rbegin(); i != Script.rend(); ++i)//C++11
				{
					m_TaskScript.push_front(std::move(*i));
				}
			}
			else
			{
				OnTaskScriptFailed(m_TaskScriptCaseRspWrongLast);
				return true;
			}

			return true;
		}
	}

	return false;
}

}
