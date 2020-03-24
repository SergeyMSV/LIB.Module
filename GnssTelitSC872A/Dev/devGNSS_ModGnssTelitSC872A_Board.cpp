#include "devGNSS.h"

namespace dev
{

tGNSS::tModGnssTelitSC872A::tBoard::tBoard(tModGnssTelitSC872A* obj, boost::asio::io_context& io)
	:m_pObj(obj), tCommunication(io, g_ConfigINI.ComPortID, g_ConfigINI.ComPortBR)
{

}

tGNSS::tModGnssTelitSC872A::tBoard::~tBoard()
{

}

void tGNSS::tModGnssTelitSC872A::tBoard::OnReceived(utils::tVectorUInt8& data)
{
	m_pObj->OnReceived(data);
}

}
