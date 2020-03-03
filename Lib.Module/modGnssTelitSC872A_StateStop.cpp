#include "modGnssTelitSC872A.h"

namespace mod
{

tGnssTelitSC872A::tStateStop::tStateStop(tObjectState* obj, const std::string& value)
	:tState(obj)
{
	GetObject<tGnssTelitSC872A>()->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateStop: %s", value.c_str());

	GetObject<tGnssTelitSC872A>()->m_pDataSet->SetDataValue1("tState-Stop");
}

void tGnssTelitSC872A::tStateStop::operator()()
{
	auto TimeStart = std::chrono::high_resolution_clock::now();

	GetObject<tGnssTelitSC872A>()->m_pLog->Write(false, utils::tLogColour::Yellow, "[");

	while (true)//Step 1
	{
		//do some work...
		{
			GetObject<tGnssTelitSC872A>()->m_pLog->Write(false, utils::tLogColour::LightYellow, ".");
		}

		auto TimeNow = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double, std::milli> TimeSpan = TimeNow - TimeStart;

		if (TimeSpan.count() > 100)
		{
			//Exit with ERROR...
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	GetObject<tGnssTelitSC872A>()->m_pLog->Write(false, utils::tLogColour::Yellow, "]");

	if (++m_Counter > 10)
	{
		GetObject<tGnssTelitSC872A>()->m_pLog->WriteLine();

		ChangeState(new tStateHalt(GetObject<utils::pattern_State::tObjectState>(), "eyeyeyeyey"));
		return;
	}

	GetObject<tGnssTelitSC872A>()->m_pLog->Write(false, utils::tLogColour::LightRed, "h");
}

}