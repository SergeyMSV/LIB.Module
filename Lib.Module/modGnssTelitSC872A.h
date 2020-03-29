///////////////////////////////////////////////////////////////////////////////////////////////////
// modGnssTelitSC872A.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 03 03  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <modGnssTelitSC872A_Attribute.h>

#include <utilsLog.h>
#include <utilsPacketNMEA.h>
#include <utilsPacketNMEAPayload.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <queue>

namespace mod
{

typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEA, utils::packet_NMEA::tPayloadCommon> tPacketNMEA;
typedef utils::packet_NMEA::tPayloadRMC<13, 3, 4, 4> tMsgRMC;
typedef utils::packet_NMEA::tPayloadGSV tMsgGSV;

class tGnssTelitSC872A
{
	using tClock = std::chrono::high_resolution_clock;//C++11

	class tState
	{
	protected:
		tGnssTelitSC872A* m_pObj = nullptr;

		int m_Counter = 0;//[TBD] TEST

	public:
		tState(tGnssTelitSC872A* obj);

		virtual bool operator()() { return false; }

		virtual bool Start() { return false; }
		virtual bool Halt();

		virtual tGnssTelitSC872AStatus GetStatus() = 0;

		void ChangeState(tState* state) { m_pObj->ChangeState(state); }
	};

	class tStateError :public tState
	{
	public:
		tStateError(tGnssTelitSC872A* obj, const std::string& value);

		bool operator()() override;

		tGnssTelitSC872AStatus GetStatus() override { return tGnssTelitSC872AStatus::Error; }
	};

	class tStateHalt :public tState
	{
	public:
		tStateHalt(tGnssTelitSC872A* obj, const std::string& value);

		bool operator()() override;

		bool Start() override { return false; }
		bool Halt() override { return true; }

		tGnssTelitSC872AStatus GetStatus() override { return tGnssTelitSC872AStatus::Halted; }
	};

	class tStateOperation :public tState
	{
		utils::tVectorUInt8 m_ReceivedData;
		std::chrono::time_point<tClock> m_StartTime;

		bool m_NoData = true;

		tGnssTelitSC872ADataSet m_DataSet;

		int m_TimeCounter = 0;

		tStateOperation(tGnssTelitSC872A* obj, const std::chrono::time_point<tClock>& startTime, bool noData);

	public:
		tStateOperation(tGnssTelitSC872A* obj);
		
		bool operator()() override;

		tGnssTelitSC872AStatus GetStatus() override { return tGnssTelitSC872AStatus::Operation; }
	};

	class tStateStart :public tState
	{

	public:
		tStateStart(tGnssTelitSC872A* obj, const std::string& value);

		bool operator()() override;

		tGnssTelitSC872AStatus GetStatus() override { return tGnssTelitSC872AStatus::Init; }
	};

	class tStateStop :public tState
	{
	public:
		tStateStop(tGnssTelitSC872A* obj, const std::string& value);

		bool operator()() override;

		bool Start() override { return false; }
		bool Halt() override { return true; }

		tGnssTelitSC872AStatus GetStatus() override { return tGnssTelitSC872AStatus::Deinit; }
	};

	//mutable std::mutex m_Mtx;

	utils::tLog* m_pLog = nullptr;

	tState* m_pState = nullptr;

	std::atomic_bool m_Control_Operation = false;
	std::atomic_bool m_Control_Restart = false;
	std::atomic_bool m_Control_Exit = false;

	//tGnssTelitSC872AControl m_Control;
	tGnssTelitSC872ASettings m_Settings;
	tGnssTelitSC872AProperty m_Property;

	mutable std::mutex m_MtxReceivedData;
	std::queue<utils::tVectorUInt8> m_ReceivedData;

	//tGnssTelitSC872ADataSet* m_pDataSet = nullptr;

public:
	tGnssTelitSC872A() = delete;
	tGnssTelitSC872A(utils::tLog* log, const tGnssTelitSC872ASettings& settings, bool start = false);
	tGnssTelitSC872A(const tGnssTelitSC872A&) = delete;
	tGnssTelitSC872A(tGnssTelitSC872A&&) = delete;

	tGnssTelitSC872AError operator()();

	void Start();
	void Restart();
	void Halt();
	void Exit();

	tGnssTelitSC872AStatus GetStatus();

	//tGnssTelitSC872ASettings GetSettings();
	//void SetSettings(const tGnssTelitSC872ASettings& settings);

protected:
	virtual void OnChanged(const tGnssTelitSC872ADataSet& value) = 0;
	//virtual void OnChanged(const tGnssTelitSC872AProperty& value) = 0;
	//virtual void OnFailed(const tGnssTelitSC872AError& value) = 0;

	//virtual void OnStateStart() = 0;
	//virtual void OnStateResponse() = 0;
	//virtual void OnStateReady() = 0;
	//virtual void OnStateHalt() = 0;
	//virtual void OnStateRestart() = 0;

	//virtual void OnGGA(GnssMTK::PacketNMEA::tMsgGGA value) { }
	//virtual void OnGSV(GnssMTK::PacketNMEA::tMsgGSV value) { }
	//virtual void OnRMC(GnssMTK::PacketNMEA::tMsgRMC value) { }

//protected:
//	virtual void Board_PowerSupply(bool state) = 0;
//	virtual void Board_Reset(bool state) = 0;
//
//	virtual bool Board_Send(std::vector<char>& data) = 0;
	void Board_OnReceived(utils::tVectorUInt8& data);

private:
	bool IsReceivedData();
	utils::tVectorUInt8 GetReceivedDataChunk();
	bool IsControlOperation() { return m_Control_Operation && !m_Control_Restart; }
	//bool IsControlStop() { return !m_Control_Operation && m_Control_Restart; }
	bool IsControlRestart() { return m_Control_Restart; }
	bool IsControlHalt() { return !m_Control_Operation; }

	void ClearReceivedData();

	void SetStrTimePeriod(std::stringstream& stream, const std::chrono::time_point<tClock>& timePoint) const;
	void SetStrBaudrate(std::stringstream& stream, const std::chrono::time_point<tClock>& timePoint, std::size_t sizeBytes) const;

	void ChangeState(tState* state);
};

}
