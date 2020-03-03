#include "modGnssTelitSC872A.h"

namespace mod
{

tGnssTelitSC872A::tStateError::tStateError(tObjectState* obj, const std::string& value)
	:tState(obj)
{
	GetObject<tGnssTelitSC872A>()->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateError: %s", value.c_str());

	GetObject<tGnssTelitSC872A>()->m_pDataSet->SetDataValue1("tState-Operation");
}

void tGnssTelitSC872A::tStateError::operator()()
{
	if (++m_Counter < 10)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	else
	{
		GetObject<tGnssTelitSC872A>()->m_pLog->WriteLine();

		ChangeState(new tStateStop(GetObject<utils::pattern_State::tObjectState>(), "lalala"));
		return;
	}

	GetObject<tGnssTelitSC872A>()->m_pLog->Write(false, utils::tLogColour::LightRed, "e");
}

}
