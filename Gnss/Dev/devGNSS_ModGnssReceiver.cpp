#include "devGNSS.h"
#include "devDataSet.h"
#include "devSettings.h"

#define BOOST_JSON_STACK_BUFFER_SIZE 1024
#include <boost/json/src.hpp>

namespace dev
{

tGNSS::tModGnssReceiver::tModGnssReceiver(tGNSS* obj)
	:mod::tGnssReceiver(obj->m_pLog), m_pObj(obj), m_Board(this, *obj->m_pIO)
{

}

tGNSS::tModGnssReceiver::~tModGnssReceiver()
{

}

mod::tGnssTaskScript tGNSS::tModGnssReceiver::GetTaskScript(const std::string& id, bool userTaskScript)
{
	return g_Settings.GetTaskScript(id, userTaskScript);
}

std::string tGNSS::tModGnssReceiver::GetNMEA_MsgLast()
{
	return g_Settings.GetNMEA_MsgLast();
}

void tGNSS::tModGnssReceiver::OnChanged(const mod::tGnssDataSet& value)
{
	/*std::string Timestamp = db::GetTimestampNow();
	std::string DateTime = db::ToString(value.GetDateTime());

	my_ulonglong Res = db::InsertTablePos(Timestamp, static_cast<char>(value.GNSS), DateTime, value.Valid, value.Latitude, value.Longitude, value.Altitude, value.Speed, value.Course);

	db::tTableSatBulk TableSat(value.Satellite.size());

	auto SatIter = value.Satellite.cbegin();
	for (std::size_t i = 0; i < TableSat.size() && SatIter != value.Satellite.cend(); ++i, ++SatIter)
	{
		TableSat[i].pos_id = Res;
		TableSat[i].sat_id = SatIter->ID.Value;
		TableSat[i].elevation = SatIter->Elevation.Value;
		TableSat[i].azimuth = SatIter->Azimuth.Value;
		TableSat[i].snr = SatIter->SNR.Value;
	}

	db::InsertTablePosSatBulk(TableSat);*/

	//my_ulonglong InsertTablePos(const std::string& timestamp, char gnss, const std::string& dateTime, bool valid, double latitude, double longitude, double altitude, double speed, double course)
//{
//	const tSQLQueryParam Query
//	{
//		{"timestamp", timestamp},
//		{"gnss", ToString(gnss)},
//		{"date_time", dateTime},
//		{"valid", ToString(valid)},
//		{"latitude", ToString(latitude)},
//		{"longitude", ToString(longitude)},
//		{"altitude", ToString(altitude)},
//		{"speed", ToString(speed)},
//		{"course", ToString(course)},
//		{"rcv_id", ToString(g_MySQL.RcvID)},
//		{"update_id", ToString(g_MySQL.UpdateID)},
//	};
//
//	return Insert("pos", Query);
//}

//my_ulonglong InsertTablePosSat(int pos_id, int sat_id, int elevation, int azimuth, int snr, int& cerr)
//{
//	const tSQLQueryParam Query
//	{
//		{"pos_id", ToString(pos_id)},
//		{"sat_id", ToString(sat_id)},
//		{"elevation", ToString(elevation)},
//		{"azimuth", ToString(azimuth)},
//		{"snr", ToString(snr)},
//	};
//
//	return Insert("pos_sat", Query, cerr);
//}


	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, value.ToString());
}

void tGNSS::tModGnssReceiver::Board_PowerSupply(bool state)
{
	std::stringstream SStream;
	SStream << "Board_PowerSupply: " << state;
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightMagenta, SStream.str());
}

void tGNSS::tModGnssReceiver::Board_Reset(bool state)
{
	std::stringstream SStream;
	SStream << "Board_Reset:       " << state;
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightMagenta, SStream.str());
}

bool tGNSS::tModGnssReceiver::Board_Send(const utils::tVectorUInt8& data)
{
	return m_Board.Send(data);
}

void tGNSS::tModGnssReceiver::OnReceived(utils::tVectorUInt8& data)
{
	Board_OnReceived(data);
}

}
