#include "modFiniteStateMachine.h"

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

	GetObject<tFiniteStateMachine>()->m_pLog->Write(false, utils::tLogColour::LightRed, "o");
}

}

