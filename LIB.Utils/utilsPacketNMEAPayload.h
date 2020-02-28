///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAPayload.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 01 27  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsBase.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <vector>

namespace utils
{
	namespace packet_NMEA
	{
		namespace Payload
		{
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tDate
{
	tUInt8 Year = 0;
	tUInt8 Month = 0;
	tUInt8 Day = 0;

	tDate(tUInt8 year, tUInt8 month, tUInt8 day);
	explicit tDate(const std::string& val);	

	std::string ToString();
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tTime
{
	tUInt8 Hour = 0;
	tUInt8 Minute = 0;
	tUInt8 Second = 0;

	tTime(tUInt8 hour, tUInt8 minute, tUInt8 second);
	explicit tTime(const std::string& val);

	std::string ToString();
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Size>
struct tLatitude
{
	double Value = 0;

	explicit tLatitude(double val) :Value(val) { }
	explicit tLatitude(const std::string& val)
	{
		static_assert(Size > 2, "tLatitude: Size");//C++11

		if (val.size() == Size)
		{
			char Data[3]{};

			std::strncpy(Data, val.c_str(), sizeof(Data) - 1);

			Value = std::strtod(Data, 0, 10);//degrees - first 2 figures dd

			double Rest = std::strtod(val.c_str() + 2, 0);

			Value += Rest / 60;
		}
	}

	std::string ToString()
	{
		//if (Year < 100 && Month < 13 && Day < 32)
		//{
		//	char Str[10]{};
		//	std::sprintf(Str, "%02d%02d%02d", Day, Month, Year);
		//	return Str;
		//}

		//return "";
		////Size
		return "";
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Size>
struct tLongitude
{
	double Value = 0;

	explicit tLongitude(double val) :Value(val) { }
	explicit tLongitude(const std::string& val)
	{
		static_assert(Size > 2, "tLongitude: Size");//C++11

		if (val.size() == Size)
		{
			char Data[4]{};

			std::strncpy(Data, val.c_str(), sizeof(Data) - 1);

			Value = std::strtod(Data, 0, 10);

			double Rest = std::strtod(val.c_str() + 2, 0);

			Value += Rest / 60;
		}
	}

	std::string ToString()
	{
		//if (Year < 100 && Month < 13 && Day < 32)
		//{
		//	char Str[10]{};
		//	std::sprintf(Str, "%02d%02d%02d", Day, Month, Year);
		//	return Str;
		//}

		//return "";
		////Size
		return "";
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}
