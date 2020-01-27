///////////////////////////////////////////////////////////////////////////////////////////////////
// devFiniteStateMachine.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 01 14  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>
#include <devLog.h>

#include <modFiniteStateMachine.h>

namespace dev
{

class tDevFiniteStateMachine
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tModFiniteStateMachine : public mod::tFiniteStateMachine
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
		//class tBoard : public board::tBoardModFSMachine
		//{
		//	tModFiniteStateMachine* p_obj;

		//public:
		//	tBoard(tModFiniteStateMachine* obj);
		//	virtual ~tBoard();

		//protected:
		//	virtual void OnReceived(std::vector<char>& data);
		//};
///////////////////////////////////////////////////////////////////////////////////////////////////
		tDevFiniteStateMachine* m_pObj;

		//tBoard* m_Board;

	public:
		tModFiniteStateMachine(tDevFiniteStateMachine* obj, const mod::tFiniteStateMachineSettings& settings);
		virtual ~tModFiniteStateMachine();

//		virtual void Control();
//
//	protected:
//		virtual void Board_PowerSupply(bool state);
//		virtual void Board_Reset(bool state);
//
//		virtual bool Board_Send(std::vector<char>& data);
//
//		void OnReceived(std::vector<char>& data);
//
//		virtual void OnChanged(mod::tGnssMTKProperty value);
//
//		virtual void OnStart();
//		virtual void OnResponse();
//		virtual void OnReady();
//		virtual void OnHalt();
//		virtual void OnRestart();
//		virtual void OnFailed(mod::tGnssMTKError cerr);
//
//		virtual void OnGGA(mod::GnssMTK::PacketNMEA::tMsgGGA value);
//		virtual void OnGSV(mod::GnssMTK::PacketNMEA::tMsgGSV value);
//		virtual void OnRMC(mod::GnssMTK::PacketNMEA::tMsgRMC value);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	friend class tModFiniteStateMachine;

	utils::tLog *m_pLog = nullptr;

	mod::tFiniteStateMachineDataSet m_DataSet;

	tModFiniteStateMachine *m_pModFSMachine = nullptr;
//
public:
	tDevFiniteStateMachine() = delete;
	explicit tDevFiniteStateMachine(utils::tLog* log);
	tDevFiniteStateMachine(const tDevFiniteStateMachine&) = delete;
	tDevFiniteStateMachine(tDevFiniteStateMachine&&) = delete;
	~tDevFiniteStateMachine();

	void operator()();

	void Start();
	void Halt();

	mod::tFiniteStateMachineStatus GetStatus();

	mod::tFiniteStateMachineDataSet* GetDataSet()//[TBD] STUPID, THAT'S WHY HERE
	{
		return &m_DataSet;
	}
//
//	void Tick10ms();
//
//	void Control();
//
//	void Start();
//	void Halt();
//
//	void Reset(mod::tGnssMTKReset resetType);
//
//	void SetSettings(mod::tGnssMTKSettings settings);
//
//	bool SetSerialPort(int baudrate);
};

}
