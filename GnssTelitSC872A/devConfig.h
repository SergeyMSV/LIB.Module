#pragma once

#include <utilsBase.h>

#include <atomic>
#include <string>

struct tConfigINI
{
	std::string ComPortID;
	utils::tUInt32 ComPortBR = 0;
};

extern tConfigINI g_ConfigINI;


struct tDataSetMainControl
{
	enum class tStateGNSS
	{
		Nothing,
		Halt,
		Start,
		Restart,
		Exit,
	};

	std::atomic<tStateGNSS> Thread_GNSS_State = tStateGNSS::Halt;
};