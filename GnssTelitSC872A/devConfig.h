#pragma once

#include <utilsBase.h>

#include <atomic>
#include <string>

#define DEV_DB_VERSION "DuperDBase 0.0.1"

struct tConfigINI
{
	struct
	{
		std::string Model;
		std::string Ident;
	}Main;

	struct
	{
		std::string ID;
		utils::tUInt32 BR = 0;
	}SerialPort;

	struct
	{
		std::string Host;
		std::string User;
		std::string Passwd;
		std::string DB;
		unsigned int Port = 0;
	}DB;

	std::string ConfigFileName;
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