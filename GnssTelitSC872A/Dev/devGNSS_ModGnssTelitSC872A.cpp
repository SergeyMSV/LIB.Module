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
	auto PosID = db::InsertTablePos(Timestamp, static_cast<char>(value.GNSS), DateTime, value.Valid, value.Latitude, value.Longitude, value.Altitude, value.Speed, value.Course, 123, Cerr);

	for (auto& sat : value.Satellite)
	{
		db::InsertTableSat(PosID, sat.ID.Value, sat.Elevation.Value, sat.Azimuth.Value, sat.SNR.Value, Cerr);
	}

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, value.ToString());
}

void tGNSS::tModGnssTelitSC872A::OnReceived(utils::tVectorUInt8& data)
{
	Board_OnReceived(data);
}

}
