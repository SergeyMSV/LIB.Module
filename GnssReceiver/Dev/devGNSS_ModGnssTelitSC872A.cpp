#include "devGNSS.h"
#include "devDataSet.h"
#include "devDB.h"
#include "devSettings.h"

namespace dev
{

tGNSS::tModGnssReceiver::tModGnssReceiver(tGNSS* obj, const mod::tGnssReceiverSettings& settings)
	:mod::tGnssReceiver(obj->m_pLog, settings), m_pObj(obj), m_Board(this, *obj->m_pIO)
{

}

tGNSS::tModGnssReceiver::~tModGnssReceiver()
{

}

void tGNSS::tModGnssReceiver::OnChanged(const mod::tGnssDataSet& value)
{
	std::string Timestamp = db::GetTimestampNow();
	std::string DateTime = db::ToString(value.GetDateTime());
	int Cerr = 0;
	auto PosID = db::InsertTablePos(Timestamp, static_cast<char>(value.GNSS), DateTime, value.Valid, value.Latitude, value.Longitude, value.Altitude, value.Speed, value.Course, Cerr);

	if (!Cerr)
	{
		db::tTableSatBulk TableSat(value.Satellite.size());

		auto SatIter = value.Satellite.cbegin();
		for (std::size_t i = 0; i < TableSat.size() && SatIter != value.Satellite.cend(); ++i, ++SatIter)
		{
			TableSat[i].pos_id = PosID;
			TableSat[i].sat_id = SatIter->ID.Value;
			TableSat[i].elevation = SatIter->Elevation.Value;
			TableSat[i].azimuth = SatIter->Azimuth.Value;
			TableSat[i].snr = SatIter->SNR.Value;
		}

		db::InsertTablePosSatBulk(TableSat, Cerr);
	}

	if (Cerr)
	{
		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightRed, "ERRRRRRRRRRRRRRR");//[TBD]
	}

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, value.ToString());
}

void tGNSS::tModGnssReceiver::OnReceived(utils::tVectorUInt8& data)
{
	Board_OnReceived(data);
}

}
