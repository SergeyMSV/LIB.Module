#include "modGnssTelitSC872A_Attribute.h"

namespace mod
{

bool tGnssTelitSC872ADataSet::Changed()
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	if (m_Changed)
	{
		m_Changed = false;
		return true;
	}

	return false;
}

std::string tGnssTelitSC872ADataSet::GetDataValue1() const
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	return m_DataValue1;
}

void tGnssTelitSC872ADataSet::SetDataValue1(const std::string& value)
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	if (!m_Changed && m_DataValue1 != value)
	{
		m_Changed = true;
	}

	m_DataValue1 = value;
}

}