#include "devLog.h"

#include <iostream>

namespace dev
{

tLog::tSettings tLog::LogSettings;

tLog::tLog(unsigned int id)
	:utils::tLog(true)
{
	m_Id = id;
}

void tLog::WriteLog(const std::string& msg)
{
	if (LogSettings.Value & m_Id)
	{
		std::cout << msg;
	}
}

}
