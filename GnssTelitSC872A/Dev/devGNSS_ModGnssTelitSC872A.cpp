#include "devGNSS.h"

namespace dev
{

tGNSS::tModGnssTelitSC872A::tModGnssTelitSC872A(tGNSS* obj, const mod::tGnssTelitSC872ASettings& settings)
	:mod::tGnssTelitSC872A(obj->m_pLog, settings, &obj->m_DataSet), m_pObj(obj)
{
	//m_Board = 0;

	//m_TestGNSS = new utils::test::GNSS::tTestGNSS(g_DefaultSettings_Files_LogFileName);
}

tGNSS::tModGnssTelitSC872A::~tModGnssTelitSC872A()
{

}

//tGnss::tModGnss::~tModGnss()
//{
//	if (m_Board)
//		delete m_Board;
//
//	delete m_TestGNSS;
//}
//
//void tGnss::tModGnss::Control()
//{
//	if (m_Board)
//		m_Board->Control();
//
//	mod::tGnssMTK::Control();
//}
//
//void tGnss::tModGnss::Board_PowerSupply(bool state)
//{
//	if (m_Board)
//		m_Board->PowerSupply(state);
//}
//
//void tGnss::tModGnss::Board_Reset(bool state)
//{
//	if (m_Board)
//		m_Board->Reset(state);
//}
//
//bool tGnss::tModGnss::Board_Send(std::vector<char>& data)
//{
//	if (m_Board)
//		return m_Board->Send(data);
//
//	return false;
//}
//
//void tGnss::tModGnss::OnReceived(std::vector<char>& data)
//{
//	Board_OnReceived(data);
//}
//
//void tGnss::tModGnss::OnChanged(mod::tGnssMTKProperty value)
//{
//	p_obj->p_log->WriteLine("OnChanged");
//
//	p_obj->p_log->WriteLine(">MODEL: " + value.Model);
//}
//
//void tGnss::tModGnss::OnStart()
//{
//	p_obj->p_log->WriteLine("OnStart");
//
//	if (m_Board == 0)
//	{
//		m_Board = new tBoard(this);
//	}
//}
//
//void tGnss::tModGnss::OnResponse()
//{
//	p_obj->p_log->WriteLine("OnResponse");
//}
//
//void tGnss::tModGnss::OnReady()
//{
//	p_obj->p_log->WriteLine("OnReady");
//}
//
//void tGnss::tModGnss::OnHalt()
//{
//	p_obj->p_log->WriteLine("OnHalt");
//
//	if (m_Board)
//	{
//		tBoard *Board = m_Board;
//
//		m_Board = 0;
//
//		delete Board;
//	}
//}
//
//void tGnss::tModGnss::OnRestart()
//{
//	p_obj->p_log->WriteLine("OnRestart");
//
//	if (m_Board)
//	{
//		tBoard *Board = m_Board;
//
//		m_Board = 0;
//
//		delete Board;
//	}
//}
//
//void tGnss::tModGnss::OnFailed(mod::tGnssMTKError cerr)
//{
//	p_obj->p_log->WriteLine("OnFailed: 0x" + utils::ToString((unsigned int)cerr, utils::tRadix_16));
//}
//
//void tGnss::tModGnss::OnGGA(mod::GnssMTK::PacketNMEA::tMsgGGA value)
//{
//
//}
//
//void tGnss::tModGnss::OnGSV(mod::GnssMTK::PacketNMEA::tMsgGSV value)
//{
//	utils::test::GNSS::NMEA::tMsgGSV MsgGSV;
//
//	MsgGSV.MessageNumber = value.GetMessageNumber();
//	MsgGSV.MessagesQty = value.GetMessageQty();
//	MsgGSV.SatellitesInViewQty = value.GetSatellitesInViewQty();
//
//	utils::test::GNSS::NMEA::tMsgGSV::tSatellite Satellite;
//
//	std::vector<mod::GnssMTK::PacketNMEA::tMsgGSV::tSatellite> GSV_Satellite = value.GetSatellites();
//
//	for (unsigned int i = 0; i < GSV_Satellite.size(); ++i)
//	{
//		Satellite.SatelliteID = GSV_Satellite[i].SatelliteID;
//		Satellite.Azimuth = GSV_Satellite[i].Azimuth;
//		Satellite.Elevation = GSV_Satellite[i].Elevation;
//		Satellite.SignalNoiseRatio = GSV_Satellite[i].SignalNoiseRatio;
//
//		MsgGSV.Satellite.push_back(Satellite);
//	}
//
//	m_TestGNSS->OnReceived_MsgGSV(MsgGSV);
//
//	//if (MsgGSV.MessageNumber == MsgGSV.MessagesQty)
//	//{
//	//	m_TestGNSS->OnReceived_Msg_Last();//For certain receiver
//	//}
//}
//
//void tGnss::tModGnss::OnRMC(mod::GnssMTK::PacketNMEA::tMsgRMC value)
//{
//	utils::test::GNSS::NMEA::tMsgRMC MsgRMC;
//
//	MsgRMC.Date = value.GetDate();
//	MsgRMC.Time = value.GetTime();
//	MsgRMC.Validity = value.IsValid();
//
//	m_TestGNSS->OnReceived_MsgRMC(MsgRMC);
//
//	m_TestGNSS->OnReceived_Msg_Last();//For certain receiver
//}
//
}
