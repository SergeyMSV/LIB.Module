#include "devGNSS.h"
#include "devDataSet.h"
#include "devDB.h"

namespace dev
{

tGNSS::tModGnssTelitSC872A::tModGnssTelitSC872A(tGNSS* obj, const mod::tGnssTelitSC872ASettings& settings)
	:mod::tGnssTelitSC872A(obj->m_pLog, settings), m_pObj(obj), m_Board(this, *obj->m_pIO)
{

}

tGNSS::tModGnssTelitSC872A::~tModGnssTelitSC872A()
{

}

void tGNSS::tModGnssTelitSC872A::OnChanged(const mod::tGnssTelitSC872ADataSet& value)
{
	std::string Timestamp = db::GetTimestampNow();
	std::string DateTime = db::GetTimestamp(value.Year, value.Month, value.Day, value.Hour, value.Minute, static_cast<unsigned char>(value.Second));
	int Cerr = 0;
	auto PosID = db::InsertTablePos(Timestamp, static_cast<char>(value.GNSS), DateTime, value.Valid, value.Latitude, value.Longitude, value.Altitude, value.Speed, value.Course, Cerr);

	if (!Cerr)
	{
		db::tTableSatBulk TableSat(value.Satellite.size());

		auto SatIter = value.Satellite.cbegin();
		for (std::size_t i = 0; i < TableSat.size() && SatIter != value.Satellite.cend(); ++i, ++SatIter)
		{
			TableSat[i].pos_id = PosID;
			TableSat[i].sat_id = (*SatIter).ID.Value;
			TableSat[i].elevation = (*SatIter).Elevation.Value;
			TableSat[i].azimuth = (*SatIter).Azimuth.Value;
			TableSat[i].snr = (*SatIter).SNR.Value;
			//TableSat[i].sat_id = value.Satellite[i].ID.Value;
			//TableSat[i].elevation = value.Satellite[i].Elevation.Value;
			//TableSat[i].azimuth = value.Satellite[i].Azimuth.Value;
			//TableSat[i].snr = value.Satellite[i].SNR.Value;
		}

		db::InsertTablePosSatBulk(TableSat, Cerr);
	}

	if (Cerr)
	{
		m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightRed, "ERRRRRRRRRRRRRRR");//[TBD]
	}

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, value.ToString());
}

void tGNSS::tModGnssTelitSC872A::OnReceived(utils::tVectorUInt8& data)
{
	Board_OnReceived(data);
}

}
