#include "modGnssReceiver.h"

namespace mod
{

tGnssReceiver::tStateStop::tStateStop(tGnssReceiver* obj, const std::string& value)
	:tState(obj, "StateStop")
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateStop: %s", value.c_str());
}

void tGnssReceiver::tStateStop::OnTaskScriptDone()
{
	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnTaskScriptDone");

	ChangeState(new tStateHalt(m_pObj, "stop"));
	return;
}

void tGnssReceiver::tStateStop::OnTaskScriptFailed()
{
	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnTaskScriptFailed");

	ChangeState(new tStateStop(m_pObj, "stop"));//[TBD] cycle
	return;
}

void tGnssReceiver::tStateStop::OnTaskScriptFailed(const std::string& msg)
{
	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnTaskScriptFailed: " + msg);

	ChangeState(new tStateStop(m_pObj, "stop"));//[TBD] cycle
	return;
}

void tGnssReceiver::tStateStop::Go()
{

}

void tGnssReceiver::tStateStop::OnReceived(const tPacketNMEA_Template& value)
{
	std::string Payload = value.GetPayload();

	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnReceived: " + Payload);
}

}