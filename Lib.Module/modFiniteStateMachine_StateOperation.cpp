#include "modFiniteStateMachine.h"

#include <utilsPacketNMEA.h>

#include <chrono>
#include <thread>

namespace mod
{

tFiniteStateMachine::tStateOperation::tStateOperation(tObjectState* obj, const std::string& value)
	:tState(obj)
{
	GetObject<tFiniteStateMachine>()->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateOperation: %s", value.c_str());

	GetObject<tFiniteStateMachine>()->m_pDataSet->SetDataValue1("tState-Operation");
}

void tFiniteStateMachine::tStateOperation::operator()()
{
	if (++m_Counter < 10)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//std::this_thread::sleep_for(std::chrono::duration<unsigned long, std::milli>(1000));
	}
	else
	{
		GetObject<tFiniteStateMachine>()->m_pLog->WriteLine();

		ChangeState(new tStateError(GetObject<utils::pattern_State::tObjectState>(), "lalala"));
		//ChangeState(new tStateStop(GetObject<utils::pattern_State::tObjectState>(), "lalala"));
		return;
	}

	std::string Data = "$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa";

	utils::tVectorUInt8 DataVector(Data.cbegin(), Data.cend());

	tPacketNMEA Packet;

	if (tPacketNMEA::Find(DataVector, Packet))
	{
		GetObject<tFiniteStateMachine>()->m_pLog->Write(true, utils::tLogColour::LightYellow, "");

		std::vector<std::string> PacketData = Packet.GetPayload();

		for (std::string i : PacketData)
		{
			GetObject<tFiniteStateMachine>()->m_pLog->Write(false, utils::tLogColour::LightYellow, i + " ");
		}

		GetObject<tFiniteStateMachine>()->m_pLog->WriteLine();
	}
	else
	{
		GetObject<tFiniteStateMachine>()->m_pLog->Write(false, utils::tLogColour::LightRed, "o");
	}
}

}

