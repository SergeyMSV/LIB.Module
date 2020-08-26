///////////////////////////////////////////////////////////////////////////////////////////////////
// devConfig.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 04 29  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>

#include <utilsBase.h>

#include <string>

namespace dev
{

struct tSettings
{
	struct
	{
		std::string Model;
		std::string ID;
	}Main;

	struct
	{
		std::string ID;
		std::uint32_t BR = 0;
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

	tSettings() {}
	explicit tSettings(const std::string& fileName);
};

extern tSettings g_Settings;

}