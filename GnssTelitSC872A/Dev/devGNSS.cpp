#include "devGNSS.h"

namespace dev
{

tGNSS::tGNSS(utils::tLog* log, boost::asio::io_context& io)
	:m_pLog(log), m_pIO(&io)
{
	mod::tGnssTelitSC872ASettings Settings;

	m_pModFSMachine = new tModGnssTelitSC872A(this, Settings);
}

tGNSS::~tGNSS()
{
	delete m_pModFSMachine;
}

void tGNSS::operator()()
{
	if (m_pModFSMachine)
	{
		(*m_pModFSMachine)();
	}
}

void tGNSS::Start()
{
	if (m_pModFSMachine)
	{
		m_pModFSMachine->Start();
	}
}

void tGNSS::Restart()
{
	if (m_pModFSMachine)
	{
		m_pModFSMachine->Restart();
	}
}

void tGNSS::Halt()
{
	if (m_pModFSMachine)
	{
		m_pModFSMachine->Halt();
	}
}

void tGNSS::Exit()
{
	if (m_pModFSMachine)
	{
		m_pModFSMachine->Exit();
	}
}

mod::tGnssTelitSC872AStatus tGNSS::GetStatus()
{
	if (m_pModFSMachine)
	{
		return m_pModFSMachine->GetStatus();
	}

	return mod::tGnssTelitSC872AStatus::Unknown;
}

}
