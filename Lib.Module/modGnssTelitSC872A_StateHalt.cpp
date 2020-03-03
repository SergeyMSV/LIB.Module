#include "modGnssTelitSC872A.h"

namespace mod
{

tGnssTelitSC872A::tStateHalt::tStateHalt(tObjectState* obj, const std::string& value)
	:tState(obj)
{
	GetObject<tGnssTelitSC872A>()->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateHalt: %s", value.c_str());

	GetObject<tGnssTelitSC872A>()->m_pDataSet->SetDataValue1("tState-Halt");
}

}
