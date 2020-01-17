#include "modFiniteStateMachine.h"

#include <iostream>

namespace mod
{

tFiniteStateMachine::tFiniteStateMachine(utils::tLog* log, const tFiniteStateMachineSettings& settings, tFiniteStateMachineDataSet* dataSet)
	:m_pLog(log), m_pDataSet(dataSet)
{
	ChangeState(new tStateStart(this, "the very start"));
	return;
}

tFiniteStateMachineError tFiniteStateMachine::operator()()
{
	GetState<tState>()->DoSomeWork();

	return tFiniteStateMachineError::OK;
}

tFiniteStateMachineSettings tFiniteStateMachine::GetSettings()
{
	return m_Settings;
}

void tFiniteStateMachine::SetSettings(const tFiniteStateMachineSettings& settings)
{
	m_Settings = settings;
}

void tFiniteStateMachine::DoSomeWork_Main()
{
	std::cout << "DoSomeWork_Main\n";
}

}
