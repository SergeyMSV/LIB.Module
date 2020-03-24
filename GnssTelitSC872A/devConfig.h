#pragma once

#include <utilsBase.h>

#include <string>

struct tConfigINI
{
	std::string ComPortID;
	utils::tUInt32 ComPortBR = 0;
};

extern tConfigINI g_ConfigINI;