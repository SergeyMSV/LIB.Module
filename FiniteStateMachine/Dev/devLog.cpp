#include "devLog.h"

#include <iostream>

namespace dev
{

tLog::tSettings tLog::LogSettings;

tLog::tLog(unsigned int id)
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
