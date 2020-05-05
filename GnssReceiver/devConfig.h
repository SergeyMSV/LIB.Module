#pragma once

#include <utilsBase.h>

#include <atomic>
#include <string>

#define DEV_DB_VERSION "DuperDBase 0.0.1"

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