#include "modGnssTelitSC872A.h"

namespace mod
{

tGnssTelitSC872A::tState::tState(tObjectState* obj)
	:tStateBase(obj)
{

}

bool tGnssTelitSC872A::tState::Halt()
{
	ChangeState(new tStateStop(GetObject<utils::pattern_State::tObjectState>(), "halt"));
	return true;
}

}
