#include "modGnssTelitSC872A.h"

#include <iostream>

namespace mod
{

tGnssTelitSC872A::tGnssTelitSC872A(utils::tLog* log, const tGnssTelitSC872ASettings& settings, bool start)
	:m_pLog(log)
{
	m_Control_Operation = start;

	if (m_Control_Operation)
	{
		ChangeState(new tStateStart(this, "the very start"));
		return;
	}
	else
	{
		ChangeState(new tStateHalt(this, "the very start"));
		return;
	}
}

tGnssTelitSC872AError tGnssTelitSC872A::operator()()
{
	//std::lock_guard<std::mutex> Lock(m_Mtx);
	while ((*m_pState)());

	return tGnssTelitSC872AError::OK;
}

void tGnssTelitSC872A::Start()
{
	m_Control_Operation = true;
}

void tGnssTelitSC872A::Restart()
{
	m_Control_Restart = true;
}

void tGnssTelitSC872A::Halt()
{
	m_Control_Operation = false;
}

void tGnssTelitSC872A::Exit()
{
	m_Control_Exit = true;
	m_Control_Operation = false;
}

tGnssTelitSC872AStatus tGnssTelitSC872A::GetStatus()
{
	//std::lock_guard<std::mutex> Lock(m_Mtx);

	return m_pState->GetStatus();
}

//tGnssTelitSC872ASettings tGnssTelitSC872A::GetSettings()
//{
//	std::lock_guard<std::mutex> Lock(m_Mtx);
//
//	return m_Settings;
//}
//
//void tGnssTelitSC872A::SetSettings(const tGnssTelitSC872ASettings& settings)
//{
//	std::lock_guard<std::mutex> Lock(m_Mtx);
//
//	m_Settings = settings;
//}

void tGnssTelitSC872A::Board_OnReceived(utils::tVectorUInt8& data)
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	m_ReceivedData.push(data);
}

bool tGnssTelitSC872A::IsReceivedData()
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	return m_ReceivedData.size() > 0;
}

utils::tVectorUInt8 tGnssTelitSC872A::GetReceivedDataChunk()
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	utils::tVectorUInt8 Data(std::forward<utils::tVectorUInt8>(m_ReceivedData.front()));

	m_ReceivedData.pop();

	return Data;
}

void tGnssTelitSC872A::ClearReceivedData()
{
	while (!m_ReceivedData.empty())
	{
		m_ReceivedData.pop();
	}
}

void tGnssTelitSC872A::ChangeState(tState* state)
{
	tState* Prev = m_pState;

	m_pState = state;

	delete Prev;
}

}
