#include "devGNSS.h"
#include "devDataSet.h"
#include "devSettings.h"

#include <fstream>

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
	const std::string FileName = g_Settings.Output.Path + "/" + g_Settings.Output.FileName;
	std::fstream File = std::fstream(FileName, std::ios::out);
	if (File.is_open())
	{
		File << value.ToJSON();
		File.close();
	}

	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightYellow, value.ToJSON());
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
