///////////////////////////////////////////////////////////////////////////////////////////////////
// modGnss.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 04 28  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <utilsPacketNMEAType.h>

#include <ctime>
#include <deque>
#include <iomanip>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

namespace mod
{

struct tGnssTaskScriptCmd
{
	enum class tID : unsigned char
	{
		GPI,
		GPO,
		REQ,
	};

private:
	tID m_ID = tID::REQ;

	tGnssTaskScriptCmd() = delete;

protected:
	explicit tGnssTaskScriptCmd(tID id) :m_ID(id) {}

public:
	virtual ~tGnssTaskScriptCmd() = default;

	tID GetID() { return m_ID; }
};

struct tGnssTaskScriptCmdREQ : public tGnssTaskScriptCmd
{
	//enum class tCase_NoRsp: unsigned char//C++11
	//{
	//	Continue,
	//	Restart,
	//	Error = 0xFF,
	//};

	std::string Msg;
	std::string RspHead;
	std::string RspBody;
	std::string CaseRspWrong;
	int RspWait_us = 0;
	int Pause_us = 0;
	//tCase_NoRsp Case_NoRsp = tCase_NoRsp::Continue;

	tGnssTaskScriptCmdREQ() :tGnssTaskScriptCmd(tGnssTaskScriptCmd::tID::REQ) {}
};

struct tGnssTaskScriptCmdGPI : public tGnssTaskScriptCmd
{
	std::string ID;
	bool State = 0;
	int Wait_us = 0;

	tGnssTaskScriptCmdGPI() :tGnssTaskScriptCmd(tGnssTaskScriptCmd::tID::GPI) {}
};

struct tGnssTaskScriptCmdGPO : public tGnssTaskScriptCmd
{
	std::string ID;
	bool State = 0;
	int Pause_us = 0;

	tGnssTaskScriptCmdGPO() :tGnssTaskScriptCmd(tGnssTaskScriptCmd::tID::GPO) {}
};

typedef std::deque<std::unique_ptr<tGnssTaskScriptCmd>> tGnssTaskScript;

typedef utils::packet_NMEA::Type::tGNSS_State tGNSS_State;
typedef utils::packet_NMEA::Type::tSatellite tGNSS_Satellite;

enum class tGnssStatus : unsigned char
{
	Init,
	Operation,
	Deinit,
	Halted,
	Error,
	Unknown = 0xFF,
};

enum class tGnssError : unsigned char
{
	OK,

	State_ErrTimer,

	Unknown = 0xFF,
};

struct tGnssDataSet
{
	tGNSS_State GNSS = tGNSS_State::UNKNOWN;

	bool Check_DateTime = false;
	utils::tUInt8 Year = 0;
	utils::tUInt8 Month = 0;
	utils::tUInt8 Day = 0;
	utils::tUInt8 Hour = 0;
	utils::tUInt8 Minute = 0;
	double Second = 0;

	bool Check_Position = false;
	bool Valid = false;
	double Latitude = 0;
	double Longitude = 0;
	double Altitude = 0;
	double Speed = 0;
	double Course = 0;

	std::deque<tGNSS_Satellite> Satellite;

	tGnssDataSet() = default;

	std::tm GetDateTime() const;

	template <int SizeFractTime = 3, int SizeFract = 6>
	std::string ToString() const
	{
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
	}
};

//struct tGnssSettings
//{

//};

}
