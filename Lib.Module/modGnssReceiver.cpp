#include "modGnssReceiver.h"

#include <iostream>

namespace mod
{

tGnssReceiver::tGnssReceiver(utils::tLog* log, const tGnssReceiverSettings& settings, bool start)
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

tGnssError tGnssReceiver::operator()()
{
	//std::lock_guard<std::mutex> Lock(m_Mtx);
	while ((*m_pState)());

	return tGnssError::OK;
}

void tGnssReceiver::Start()
{
	m_Control_Operation = true;
}

void tGnssReceiver::Restart()
{
	m_Control_Restart = true;
}

void tGnssReceiver::Halt()
{
	m_Control_Operation = false;
}

void tGnssReceiver::Exit()
{
	m_Control_Exit = true;
	m_Control_Operation = false;
}

tGnssStatus tGnssReceiver::GetStatus()
{
	//std::lock_guard<std::mutex> Lock(m_Mtx);

	return m_pState->GetStatus();
}

tGnssReceiverSettings tGnssReceiver::GetSettings()
{
	std::lock_guard<std::mutex> Lock(m_MtxSettings);

	return m_Settings;
}

void tGnssReceiver::SetSettings(const tGnssReceiverSettings& settings)
{
	std::lock_guard<std::mutex> Lock(m_MtxSettings);

	m_Settings = settings;

	//[TBD] - put - need restart is needed....
}

void tGnssReceiver::Board_OnReceived(utils::tVectorUInt8& data)
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	m_ReceivedData.push(data);
}

bool tGnssReceiver::IsReceivedData()
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	return m_ReceivedData.size() > 0;
}

utils::tVectorUInt8 tGnssReceiver::GetReceivedDataChunk()
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	utils::tVectorUInt8 Data(std::forward<utils::tVectorUInt8>(m_ReceivedData.front()));

	m_ReceivedData.pop();

	return Data;
}

void tGnssReceiver::ClearReceivedData()
{
	while (!m_ReceivedData.empty())
	{
		m_ReceivedData.pop();
	}
}

void tGnssReceiver::SetStrTimePeriod(std::stringstream& stream, const std::chrono::time_point<tClock>& timePoint) const
{
	auto Time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(tClock::now() - timePoint).count();//C++11
	double Time_ms = static_cast<double>(Time_ns) / 1000000;//C++11

	stream << Time_ms << " ms";
}

void tGnssReceiver::SetStrBaudrate(std::stringstream& stream, const std::chrono::time_point<tClock>& timePoint, std::size_t sizeBytes) const
{
	auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - timePoint).count();//C++11
	double Time_ms = static_cast<double>(Time_us) / 1000;//C++11

	//stream << "; ";
	stream << sizeBytes << ", ";
	stream.setf(std::ios::fixed);
	int SizeFract = 2;
	stream << std::setw(4 + SizeFract + 1) << std::setprecision(SizeFract);
	stream << Time_ms << " ms, ";

	double Time_s = Time_ms / 1000;
	if (Time_s > 0)
	{
		stream.setf(std::ios::fixed);
		SizeFract = 2;
		stream << std::setw(4 + SizeFract + 1) << std::setprecision(SizeFract);
		stream << (static_cast<double>(sizeBytes) * 8 / Time_s) << " bps";
	}
	else
	{
		stream << "n/a";
	}
}

void tGnssReceiver::ChangeState(tState* state)
{
	tState* Prev = m_pState;

	m_pState = state;

	delete Prev;
}

}
