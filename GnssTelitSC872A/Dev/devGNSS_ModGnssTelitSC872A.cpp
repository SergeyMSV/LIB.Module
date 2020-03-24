#include "devGNSS.h"

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
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, value.ToString());
}

void tGNSS::tModGnssTelitSC872A::OnReceived(utils::tVectorUInt8& data)
{
	Board_OnReceived(data);
}

}
