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
#include <devDataSet.h>

#include <modGnssTelitSC872A.h>

#include <comSerialPort.h>

#include <future>

#include <boost/asio.hpp>

namespace dev
{

class tGNSS
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tModGnssTelitSC872A : public mod::tGnssTelitSC872A
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
		class tBoard : public tCommunication<>
		{
			tModGnssTelitSC872A* m_pObj = nullptr;

		public:
			tBoard(tModGnssTelitSC872A* obj, boost::asio::io_context& io);
			virtual ~tBoard();

		protected:
			void OnReceived(utils::tVectorUInt8& data) override;
		};
///////////////////////////////////////////////////////////////////////////////////////////////////
		tGNSS* m_pObj = nullptr;

		tBoard m_Board;

	public:
		tModGnssTelitSC872A(tGNSS* obj, const mod::tGnssTelitSC872ASettings& settings);
		virtual ~tModGnssTelitSC872A();

	protected:
		void OnChanged(const mod::tGnssTelitSC872ADataSet& value) override;

//		virtual void Control();
//
//	protected:
//		virtual void Board_PowerSupply(bool state);
//		virtual void Board_Reset(bool state);
//
//		virtual bool Board_Send(std::vector<char>& data);
//
		void OnReceived(utils::tVectorUInt8& data);
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

	boost::asio::io_context* m_pIO = nullptr;

	//mod::tGnssTelitSC872ADataSet m_DataSet;

	tModGnssTelitSC872A *m_pModFSMachine = nullptr;
//
public:
	tGNSS() = delete;
	tGNSS(utils::tLog* log, boost::asio::io_context& io, std::promise<std::string>& p);
	tGNSS(const tGNSS&) = delete;
	tGNSS(tGNSS&&) = delete;
	~tGNSS();

	void operator()();

	void Start();
	void Halt();

	mod::tGnssTelitSC872AStatus GetStatus();

	//mod::tGnssTelitSC872ADataSet* GetDataSet()//[TBD] STUPID, THAT'S WHY HERE
	//{
	//	return &m_DataSet;
	//}
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
