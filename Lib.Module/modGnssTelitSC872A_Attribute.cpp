#include "modGnssTelitSC872A_Attribute.h"

namespace mod
{


std::tm tGnssTelitSC872ADataSet::GetDateTime() const
{
	std::tm Time{};

	if (Check_DateTime)
	{
		Time.tm_year = Year + 100;
		Time.tm_mon = Month - 1;
		Time.tm_mday = Day;
		Time.tm_hour = Hour;
		Time.tm_min = Minute;
		Time.tm_sec = static_cast<int>(Second);
	}
	else//01.01.1970 - Unix Epoch
	{
		Time.tm_year = 70;
		Time.tm_mday = 1;
	}
	return Time;
}

//bool tGnssTelitSC872ADataSet::Changed()
//{
//	std::lock_guard<std::mutex> Lock(m_Mtx);
//
//	if (m_Changed)
//	{
//		m_Changed = false;
//		return true;
//	}
//
//	return false;
//}
//
//std::string tGnssTelitSC872ADataSet::GetDataValue1() const
//{
//	std::lock_guard<std::mutex> Lock(m_Mtx);
//
//	return m_DataValue1;
//}
//
//void tGnssTelitSC872ADataSet::SetDataValue1(const std::string& value)
//{
//	std::lock_guard<std::mutex> Lock(m_Mtx);
//
//	if (!m_Changed && m_DataValue1 != value)
//	{
//		m_Changed = true;
//	}
//
//	m_DataValue1 = value;
//}

}