///////////////////////////////////////////////////////////////////////////////////////////////////
// modGnssTelitSC872A_Attribute.h
//
// Standard ISO/IEC 114882, C++11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <utilsPacketNMEAType.h>

#include <mutex>
#include <string>
#include <deque>
//#include <set>//[TEST]

#include <iomanip>
#include <sstream>

namespace mod
{

typedef utils::packet_NMEA::Type::tGNSS_State tGNSS_State;
typedef utils::packet_NMEA::Type::tSatellite tGNSS_Satellite;

struct tGnssTelitSC872ADataSet
{
	tGNSS_State GNSS = tGNSS_State::UNKNOWN;

	bool Check_DateTime = false;
	utils::tUInt8 Year = 0;
	utils::tUInt8 Month = 0;
	utils::tUInt8 Day = 0;
	utils::tUInt8 Hour = 0;
	utils::tUInt8 Minute = 0;
	double Second = 0;

	bool Check_GNSS = false;
	bool Valid = false;
	double Latitude = 0;
	double Longitude = 0;
	double Altitude = 0;
	double Speed = 0;
	double Course = 0;

	//struct SatelliteCmp
	//{
	//	bool operator()(const tGNSS_Satellite& lhs, const tGNSS_Satellite& rhs) const { return lhs.ID.Value < rhs.ID.Value; }
	//};
	//std::set<tGNSS_Satellite, SatelliteCmp> Satellite;
	std::deque<tGNSS_Satellite> Satellite;

	tGnssTelitSC872ADataSet() = default;

	//tGnssTelitSC872ADataSet(tGNSS_State gnss, utils::tUInt8 year, utils::tUInt8 month, utils::tUInt8 day, utils::tUInt8 hour, utils::tUInt8 minute, double second)
	//{
	//	GNSS = gnss;

	//	Valid_DateTime = true;

	//	Year = year;
	//	Month = month;
	//	Day = day;
	//	Hour = hour;
	//	Minute = minute;
	//	Second = second;
	//}

	//tGnssTelitSC872ADataSet(tGNSS_State gnss, utils::tUInt8 year, utils::tUInt8 month, utils::tUInt8 day, utils::tUInt8 hour, utils::tUInt8 minute, double second, double latitude, double longitude, double altitude, double speed, double course)
	//	:tGnssTelitSC872ADataSet(gnss, year, month, day, hour, minute, second)//C++11
	//{
	//	Valid_GNSS = true;
	//	Latitude = latitude;
	//	Longitude = longitude;
	//	Altitude = altitude;
	//	Speed = speed;
	//	Course = course;
	//}

	//tGnssTelitSC872ADataSet(const tGnssTelitSC872ADataSet& value) = delete;
	//tGnssTelitSC872ADataSet(tGnssTelitSC872ADataSet&& value)
	//{
	//	//...
	//}

	//bool operator == (const tGnssTelitSC872ADataSet& value)
	//{
	//	return
	//		GNSS == value.GNSS &&
	//		Check_DateTime == value.Check_DateTime &&
	//		Year == value.Year &&
	//		Month == value.Month &&
	//		Day == value.Day &&
	//		Hour == value.Hour &&
	//		Minute == value.Minute &&
	//		Second == value.Second &&
	//		Check_GNSS == value.Check_GNSS &&
	//		Latitude == value.Latitude &&
	//		Longitude == value.Longitude &&
	//		Altitude == value.Altitude &&
	//		Speed == value.Speed &&
	//		Course == value.Course;
	//}

	//bool operator != (const tGnssTelitSC872ADataSet& value)
	//{
	//	return !operator==(value);
	//}

	std::string ToString() const
	{
		const int SizeFractTime = 3;
		const int SizeFract = 6;

		std::stringstream Stream;

		Stream << std::setfill('0');
		Stream << std::setw(2) << static_cast<int>(Hour);
		Stream << std::setw(2) << static_cast<int>(Minute);

		Stream.setf(std::ios::fixed);

		Stream << std::setw(2 + SizeFractTime + 1) << std::setprecision(SizeFractTime) << Second;

		Stream << " " << (Valid ? 'A' : 'V');


		Stream.setf(std::ios::fixed);

		Stream << " " << std::setw(2 + SizeFract + 1) << std::setprecision(SizeFract) << Latitude;
		Stream << " " << std::setw(2 + SizeFract + 1) << std::setprecision(SizeFract) << Longitude;
		Stream << " " << std::setw(2 + SizeFract + 1) << std::setprecision(SizeFract) << Altitude;


		Stream << " " << Satellite.size();

		return Stream.str();
		//return "LALALALALALALA~!!!! PREVED MEDVED!!!!";
	}
};

//struct tGnssTelitSC872AControl
//{
//	std::atomic_bool Halt = false;
//};

enum class tGnssTelitSC872AStatus : unsigned char
{
	Init,
	Operation,
	Deinit,
	Halted,
	Error,
	Unknown
};

union tGnssTelitSC872ASettings
{

};

struct tGnssTelitSC872AProperty
{
	union
	{
		struct
		{
			unsigned int Registration : 2;
			unsigned int : 29;

		}Field;

		unsigned int Value;
	}Status;

	tGnssTelitSC872AProperty()
	{
		Status.Value = 0;
	}

	bool operator == (const tGnssTelitSC872AProperty& val)
	{
		return Status.Value == val.Status.Value;
	}

	bool operator != (const tGnssTelitSC872AProperty& val)
	{
		return !operator==(val);
	}
};

enum class tGnssTelitSC872AError : unsigned char
{
	OK,

	State_ErrTimer,

	Unknown = 0xFF,
};

}
