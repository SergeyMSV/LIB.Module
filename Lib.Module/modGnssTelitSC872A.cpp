#include "modGnssTelitSC872A.h"

#include <iostream>

namespace mod
{

tGnssTelitSC872A::tGnssTelitSC872A(utils::tLog* log, const tGnssTelitSC872ASettings& settings, tGnssTelitSC872ADataSet* dataSet)
	:m_pLog(log), m_pDataSet(dataSet)
{
	ChangeState(new tStateStart(this, "the very start"));
	return;
}

tGnssTelitSC872AError tGnssTelitSC872A::operator()()
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	(*GetState<tState>())();

	return tGnssTelitSC872AError::OK;
}

void tGnssTelitSC872A::Start()
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	ChangeState(new tStateStart(this, "the very start"));
	return;
}

void tGnssTelitSC872A::Halt()
{
	GetState<tState>()->Halt();
	return;
}

tGnssTelitSC872AStatus tGnssTelitSC872A::GetStatus()
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	return GetState<tState>()->GetStatus();
}

tGnssTelitSC872ASettings tGnssTelitSC872A::GetSettings()
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	return m_Settings;
}

void tGnssTelitSC872A::SetSettings(const tGnssTelitSC872ASettings& settings)
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	m_Settings = settings;
}

}
