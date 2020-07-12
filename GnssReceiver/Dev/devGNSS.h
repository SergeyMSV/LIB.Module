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

//template <class TModule, class TBoard>
class tGNSS_A
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tModGnssReceiver : public mod::tGnssReceiver
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
		class tBoard : public tCommunication<>
		{
			tModGnssReceiver* m_pObj = nullptr;

		public:
			tBoard(tModGnssReceiver* obj, boost::asio::io_context& io)
				:m_pObj(obj), tCommunication(io, g_Settings.SerialPort.ID, g_Settings.SerialPort.BR)
			{
			}
			virtual ~tBoard() {}

		protected:
			void OnReceived(utils::tVectorUInt8& data) override
			{
				m_pObj->OnReceived(data);
			}
		};
///////////////////////////////////////////////////////////////////////////////////////////////////
		tGNSS_A* m_pObj = nullptr;

		tBoard m_Board;

	public:
		explicit tModGnssReceiver(tGNSS_A* obj);
		virtual ~tModGnssReceiver();

	protected:
		mod::tGnssTaskScript GetTaskScript(const std::string& id, bool userTaskScript) override;

		void OnChanged(const mod::tGnssDataSet& value) override;

		void Board_PowerSupply(bool state) override;
		void Board_Reset(bool state) override;

		bool Board_Send(const utils::tVectorUInt8& data) override;
		void OnReceived(utils::tVectorUInt8& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	friend class tModGnssReceiver;

	utils::tLog *m_pLog = nullptr;

	boost::asio::io_context* m_pIO = nullptr;

	tModGnssReceiver *m_pModFSMachine = nullptr;

public:
	tGNSS() = delete;
	tGNSS(utils::tLog* log, boost::asio::io_context& io)
		:m_pLog(log), m_pIO(&io)
	{
		m_pModFSMachine = new tModGnssReceiver(this);
	}
	tGNSS(const tGNSS&) = delete;
	tGNSS(tGNSS&&) = delete;
	~tGNSS()
	{
		delete m_pModFSMachine;
	}

	tGNSS& operator=(const tGNSS&) = delete;
	tGNSS& operator=(tGNSS&&) = delete;
	void operator()()
	{
		if (m_pModFSMachine)
		{
			(*m_pModFSMachine)();
		}
	}

	void Start()
	{
		if (m_pModFSMachine)
		{
			m_pModFSMachine->Start();
		}
	}
	void Restart()
	{
		if (m_pModFSMachine)
		{
			m_pModFSMachine->Restart();
		}
	}
	void Halt()
	{
		if (m_pModFSMachine)
		{
			m_pModFSMachine->Halt();
		}
	}
	void Exit()
	{
		if (m_pModFSMachine)
		{
			m_pModFSMachine->Exit();
		}
	}

	bool StartUserTaskScript(const std::string& taskScriptID)
	{
		if (m_pModFSMachine)
		{
			return m_pModFSMachine->StartUserTaskScript(taskScriptID);
		}

		return false;
	}

	mod::tGnssStatus GetStatus()
	{
		if (m_pModFSMachine)
		{
			return m_pModFSMachine->GetStatus();
		}

		return mod::tGnssStatus::Unknown;
	}

	//tModGnssReceiver* operator->()//[TEST]
	//{
	//	return m_pModFSMachine;
	//}
};

typedef tGNSS_A tGNSS;
//typedef tGNSS_A<mod::tGnssReceiver, int> tGNSS;

}
