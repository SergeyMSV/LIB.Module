#include "modFiniteStateMachine.h"

namespace mod
{

tFiniteStateMachine::tStateStop::tStateStop(tObjectState* obj, const std::string& value)
	:tState(obj)
{
	GetObject<tFiniteStateMachine>()->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateStop: %s", value.c_str());

	GetObject<tFiniteStateMachine>()->m_pDataSet->SetDataValue1("tState-Stop");
}

}