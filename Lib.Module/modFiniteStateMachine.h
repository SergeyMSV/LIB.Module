///////////////////////////////////////////////////////////////////////////////////////////////////
// modFiniteStateMachine.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 01 14  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <modFiniteStateMachine_Attribute.h>

#include <utilsLog.h>
#include <utilsPatternState.h>

namespace mod
{

class tFiniteStateMachine :public utils::pattern_State::tObjectState
{
private:
	class tState :public utils::pattern_State::tObjectState::tStateBase
	{
	protected:
		int m_Counter = 0;

	public:
		tState(tObjectState* obj);

		bool IsReady() { return false; }
		bool IsHalted() { return false; }

		virtual void DoSomeWork() { }
	};

	class tStateError :public tState
	{
	public:
		tStateError(tObjectState* obj, const std::string& value);

		virtual void DoSomeWork();
	};

	class tStateHalt :public tState
	{
	public:
		tStateHalt(tObjectState* obj, const std::string& value);
	};

	class tStateOperation :public tState
	{
	public:
		tStateOperation(tObjectState* obj, const std::string& value);
		
		virtual void DoSomeWork();
	};

	class tStateStart :public tState
	{

	public:
		tStateStart(tObjectState* obj, const std::string& value);

		virtual void DoSomeWork();
	};

	class tStateStop :public tState
	{
	public:
		tStateStop(tObjectState* obj, const std::string& value);
	};

	utils::tLog* m_pLog = nullptr;

	tFiniteStateMachineSettings m_Settings;
	tFiniteStateMachineProperty m_Property;

	tFiniteStateMachineDataSet* m_pDataSet = nullptr;

public:
	tFiniteStateMachine() = delete;
	explicit tFiniteStateMachine(utils::tLog* log, const tFiniteStateMachineSettings& settings, tFiniteStateMachineDataSet* dataSet);
	tFiniteStateMachine(const tFiniteStateMachine&) = delete;
	tFiniteStateMachine(tFiniteStateMachine&&) = delete;

	tFiniteStateMachineError operator()();

	//bool IsReady(); via DataSet or Property!!! or Mutexes are needed!!
	//bool IsHalted(); via DataSet or Property!!!

	tFiniteStateMachineSettings GetSettings();
	void SetSettings(const tFiniteStateMachineSettings& settings);

	//virtual void OnChanged(const tFiniteStateMachineProperty& value) = 0;
	//virtual void OnFailed(const tFiniteStateMachineError& value) = 0;

	//virtual void OnStateStart() = 0;
	//virtual void OnStateResponse() = 0;
	//virtual void OnStateReady() = 0;
	//virtual void OnStateHalt() = 0;
	//virtual void OnStateRestart() = 0;

	//virtual void OnGGA(GnssMTK::PacketNMEA::tMsgGGA value) { }
	//virtual void OnGSV(GnssMTK::PacketNMEA::tMsgGSV value) { }
	//virtual void OnRMC(GnssMTK::PacketNMEA::tMsgRMC value) { }

	void DoSomeWork_Main();
};

}
