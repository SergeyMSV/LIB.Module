///////////////////////////////////////////////////////////////////////////////////////////////////
// modGnssReceiver_Attribute.h
//
// Standard ISO/IEC 114882, C++11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "modGnss.h"

#include <mutex>
#include <string>
#include <deque>
#include <ctime>

#include <iomanip>
#include <sstream>

namespace mod
{

union tGnssReceiverSettings
{
	struct tNMEA_Out
	{
		utils::tUInt8 GLL = 0;//max 0 - 5 ()
		utils::tUInt8 GGA = 1;
		utils::tUInt8 GSA = 1;
		utils::tUInt8 GSV = 1;
		utils::tUInt8 GRS = 0;
		utils::tUInt8 GST = 0;
		utils::tUInt8 RMC = 1;
		utils::tUInt8 VTG = 1;
		utils::tUInt8 ZDA = 1;
		utils::tUInt8 MCHN = 0;//PMTKCHN
		utils::tUInt8 DTM = 0;
		utils::tUInt8 GBS = 0;
	};
};

}
