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
#include <utilsPatternState.h>

#include <mutex>

namespace mod
{

typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEA, utils::packet_NMEA::tPayloadCommon> tPacketNMEA;

class tGnssTelitSC872A :public utils::pattern_State::tObjectState
{
	class tState :public utils::pattern_State::tObjectState::tStateBase
	{
	protected:
		int m_Counter = 0;

	public:
		tState(tObjectState* obj);

		virtual void operator()() { }

		virtual bool Start() { return false; }
		virtual bool Halt();

		virtual tGnssTelitSC872AStatus GetStatus() = 0;
	};

	class tStateError :public tState
	{
	public:
		tStateError(tObjectState* obj, const std::string& value);

		void operator()() override;

		tGnssTelitSC872AStatus GetStatus() override { return tGnssTelitSC872AStatus::Error; }
	};

	class tStateHalt :public tState
	{
	public:
		tStateHalt(tObjectState* obj, const std::string& value);

		bool Start() override { return false; }
		bool Halt() override { return true; }

		tGnssTelitSC872AStatus GetStatus() override { return tGnssTelitSC872AStatus::Halted; }
	};

	class tStateOperation :public tState
	{
	public:
		tStateOperation(tObjectState* obj, const std::string& value);
		
		void operator()() override;

		tGnssTelitSC872AStatus GetStatus() override { return tGnssTelitSC872AStatus::Operation; }
	};

	class tStateStart :public tState
	{

	public:
		tStateStart(tObjectState* obj, const std::string& value);

		void operator()() override;

		tGnssTelitSC872AStatus GetStatus() override { return tGnssTelitSC872AStatus::Init; }
	};

	class tStateStop :public tState
	{
	public:
		tStateStop(tObjectState* obj, const std::string& value);

		void operator()() override;

		bool Start() override { return false; }
		bool Halt() override { return true; }

		tGnssTelitSC872AStatus GetStatus() override { return tGnssTelitSC872AStatus::Deinit; }
	};

	mutable std::mutex m_Mtx;

	utils::tLog* m_pLog = nullptr;

	tGnssTelitSC872ASettings m_Settings;
	tGnssTelitSC872AProperty m_Property;

	tGnssTelitSC872ADataSet* m_pDataSet = nullptr;

public:
	tGnssTelitSC872A() = delete;
	explicit tGnssTelitSC872A(utils::tLog* log, const tGnssTelitSC872ASettings& settings, tGnssTelitSC872ADataSet* dataSet);
	tGnssTelitSC872A(const tGnssTelitSC872A&) = delete;
	tGnssTelitSC872A(tGnssTelitSC872A&&) = delete;

	tGnssTelitSC872AError operator()();

	void Start();
	void Halt();

	tGnssTelitSC872AStatus GetStatus();

	tGnssTelitSC872ASettings GetSettings();
	void SetSettings(const tGnssTelitSC872ASettings& settings);

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
//	void Board_OnReceived(std::vector<char>& data);
};

}
