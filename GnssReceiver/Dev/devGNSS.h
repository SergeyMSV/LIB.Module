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
#include <devDataSet.h>
#include <devLog.h>

#include <modGnssReceiver.h>

#include <comSerialPort.h>

#include <future>

#include <boost/asio.hpp>

namespace dev
{

class tGNSS
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tModGnssReceiver : public mod::tGnssReceiver
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
		class tBoard : public tCommunication<>
		{
			tModGnssReceiver* m_pObj = nullptr;

		public:
			tBoard(tModGnssReceiver* obj, boost::asio::io_context& io);
			virtual ~tBoard();

		protected:
			void OnReceived(utils::tVectorUInt8& data) override;
		};
///////////////////////////////////////////////////////////////////////////////////////////////////
		tGNSS* m_pObj = nullptr;

		tBoard m_Board;

	public:
		tModGnssReceiver(tGNSS* obj, const mod::tGnssReceiverSettings& settings);
		virtual ~tModGnssReceiver();

	protected:
		void OnChanged(const mod::tGnssDataSet& value) override;

//		virtual void Board_PowerSupply(bool state);
//		virtual void Board_Reset(bool state);
//
//		virtual bool Board_Send(std::vector<char>& data);
//
		void OnReceived(utils::tVectorUInt8& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	friend class tModGnssReceiver;

	utils::tLog *m_pLog = nullptr;

	boost::asio::io_context* m_pIO = nullptr;

	tModGnssReceiver *m_pModFSMachine = nullptr;

public:
	tGNSS() = delete;
	tGNSS(utils::tLog* log, boost::asio::io_context& io);
	tGNSS(const tGNSS&) = delete;
	tGNSS(tGNSS&&) = delete;
	~tGNSS();

	void operator()();

	void Start();
	void Restart();
	void Halt();
	void Exit();

	mod::tGnssStatus GetStatus();

	tModGnssReceiver* operator->()//[TEST]
	{
		return m_pModFSMachine;
	}
};

}
