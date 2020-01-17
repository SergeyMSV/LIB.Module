#include "devFiniteStateMachine.h"

//#include <fstream>
//#include <iostream>
//
//extern std::string g_DefaultSettings_Files_LogFileName;
//extern std::string g_DefaultSettings_Settings_GNSS;
//extern std::string g_DefaultSettings_Settings_Start;
//
namespace dev
{

tDevFiniteStateMachine::tDevFiniteStateMachine(utils::tLog* log)
	:m_pLog(log)
{
	mod::tFiniteStateMachineSettings Settings;

	m_pModFSMachine = new tModFiniteStateMachine(this, Settings);
}

tDevFiniteStateMachine::~tDevFiniteStateMachine()
{
	delete m_pModFSMachine;
}

void tDevFiniteStateMachine::operator()()
{
	if (m_pModFSMachine)
	{
		(*m_pModFSMachine)();
	}
}

//tGnss::tGnss()
//{
//	p_log = new tLog(tLog::LogId_Dev_ModGnss);
//
//	mod::tGnssMTKSettings GnssSettings;
//
//	GnssSettings.Value = 0;//it set with defines in libConfig
//	//GnssSettings.Field.Nmea_GGA = 1;
//	////GnssSettings.Field.Nmea_GLL = 1;
//	////GnssSettings.Field.Nmea_GSA = 1;
//	//GnssSettings.Field.Nmea_GSV = 1;
//	//GnssSettings.Field.Nmea_RMC = 1;
//	////GnssSettings.Field.Nmea_VTG = 1;
//	////GnssSettings.Field.Nmea_ZDA = 1;
//
//	if (g_DefaultSettings_Settings_GNSS == "GPS_GLONASS")
//	{
//		GnssSettings.Field.Gnss_GLONASS = 1;
//		GnssSettings.Field.Gnss_GPS = 1;
//	}
//	else if (g_DefaultSettings_Settings_GNSS == "GPS")
//	{
//		GnssSettings.Field.Gnss_GPS = 1;
//	}
//	else if (g_DefaultSettings_Settings_GNSS == "GLONASS")
//	{
//		GnssSettings.Field.Gnss_GLONASS = 1;
//	}
//
//	m_ModGnss = new tModGnss(this, GnssSettings);
//
//	if (g_DefaultSettings_Settings_Start == "true")
//	{
//		m_ModGnss->Start();
//	}
//}
//
//tGnss::~tGnss()
//{
//	delete m_ModGnss;
//
//	delete p_log;
//}
//
//void tGnss::Tick10ms()
//{
//	m_ModGnss->Tick10ms();
//}
//
//void tGnss::Control()
//{
//	m_ModGnss->Control();
//}
//
//void tGnss::Start()
//{
//	m_ModGnss->Start();
//}
//
//void tGnss::Halt()
//{
//	m_ModGnss->Halt();
//}
//
//void tGnss::Reset(mod::tGnssMTKReset resetType)
//{
//	return m_ModGnss->Reset(resetType);
//}
//
//void tGnss::SetSettings(mod::tGnssMTKSettings settings)
//{
//	return m_ModGnss->SetSettings(settings);
//}
//
//bool tGnss::SetSerialPort(int baudrate)
//{
//	return m_ModGnss->SetSerialPort(baudrate);
//}
//
}
