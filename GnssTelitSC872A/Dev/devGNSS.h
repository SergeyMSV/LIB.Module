///////////////////////////////////////////////////////////////////////////////////////////////////
// devGNSS.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 03 03  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>
#include <devLog.h>

#include <modGnssTelitSC872A.h>

namespace dev
{

class tGNSS
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tModGnssTelitSC872A : public mod::tGnssTelitSC872A
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
		//class tBoard : public board::tBoardModFSMachine
		//{
		//	tModGnssTelitSC872A* p_obj;

		//public:
		//	tBoard(tModGnssTelitSC872A* obj);
		//	virtual ~tBoard();

		//protected:
		//	virtual void OnReceived(std::vector<char>& data);
		//};
///////////////////////////////////////////////////////////////////////////////////////////////////
		tGNSS* m_pObj;

		//tBoard* m_Board;

	public:
		tModGnssTelitSC872A(tGNSS* obj, const mod::tGnssTelitSC872ASettings& settings);
		virtual ~tModGnssTelitSC872A();

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
	friend class tModGnssTelitSC872A;

	utils::tLog *m_pLog = nullptr;

	mod::tGnssTelitSC872ADataSet m_DataSet;

	tModGnssTelitSC872A *m_pModFSMachine = nullptr;
//
public:
	tGNSS() = delete;
	explicit tGNSS(utils::tLog* log);
	tGNSS(const tGNSS&) = delete;
	tGNSS(tGNSS&&) = delete;
	~tGNSS();

	void operator()();

	void Start();
	void Halt();

	mod::tGnssTelitSC872AStatus GetStatus();

	mod::tGnssTelitSC872ADataSet* GetDataSet()//[TBD] STUPID, THAT'S WHY HERE
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
