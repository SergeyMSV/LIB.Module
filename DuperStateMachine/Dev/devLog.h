///////////////////////////////////////////////////////////////////////////////////////////////////
// devLog.h
//
// Standard ISO/IEC 114882, C++14
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 01 15  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <utilsLog.h>

namespace dev
{

class tLog : public utils::tLog
{
public:
	static const unsigned int LogId_0 = (1 << 0);
	static const unsigned int LogId_1 = (1 << 1);
	//...

	union tSettings
	{
		struct
		{
			unsigned int Log_0 : 1;
			unsigned int Log_1 : 1;
			unsigned int : 30;
		}Field;

		unsigned int Value = 0;
	};

	static tSettings LogSettings;

private:
	unsigned int m_Id;

	tLog() = delete;

public:
	tLog(unsigned int id);

protected:
	virtual void WriteLog(const std::string& msg);
};

}
