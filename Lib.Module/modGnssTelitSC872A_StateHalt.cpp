#include "modGnssTelitSC872A.h"

namespace mod
{

tGnssTelitSC872A::tStateHalt::tStateHalt(tGnssTelitSC872A* obj, const std::string& value)
	:tState(obj)
{
	m_pObj->m_pLog->WriteLine(true, utils::tLogColour::Default, "tStateHalt: %s", value.c_str());

	//m_pObj->m_pDataSet->SetDataValue1("tState-Halt");
}

}
