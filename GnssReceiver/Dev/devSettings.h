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

#include <modGnss.h>

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

private:
	std::string ConfigFileName;

public:
	tSettings() {}
	explicit tSettings(const std::string& fileName);

	std::string GetConfigFileName() { return ConfigFileName; }

	mod::tGnssTaskScript GetTaskScript(const std::string& id);
};

extern tSettings g_Settings;

}