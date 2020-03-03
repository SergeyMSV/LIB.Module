///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAPayloadPMTK.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 02 07  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsBase.h"
#include "utilsPacketNMEA.h"
#include "utilsPacketNMEAType.h"

namespace utils
{
	namespace packet_NMEA
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace hidden
{

struct tPayloadPMTK001
{
	enum class tStatus : tUInt8
	{
		InvalidCommand = 0,//0 = Invalid command / packet
		UnsupportedCommand,//1 = Unsupported command / packet type
		Failed,//2 = Valid command / packet, but action failed.
		Succeeded,//3 = Valid command / packet, and action succeeded
	};

	typedef Type::tUInt<tUInt16, 3> command_type;
	typedef Type::tUInt<tStatus, 1> status_type;

	command_type CMD;
	status_type Status;

	tPayloadPMTK001() = default;
	explicit tPayloadPMTK001(const tPayloadCommon::value_type& val)
	{
		if (Try(val))
		{
			CMD = command_type(val[1]);
			Status = status_type(val[2]);
		}
	}

	static const char* GetID() { return "PMTK001"; }

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() >= 3 && !std::strcmp(val[0].c_str(), GetID());
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GetID());
		Data.push_back(CMD.ToString());
		Data.push_back(Status.ToString());

		return Data;
	}
};

}

template <int CmdID>
struct tPayloadPMTK001 : public hidden::tPayloadPMTK001
{
	explicit tPayloadPMTK001(const tPayloadCommon::value_type& val)
		:hidden::tPayloadPMTK001(val)
	{ }

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 3 && hidden::tPayloadPMTK001::Try(val);
	}
};

template <>
struct tPayloadPMTK001<355> : public hidden::tPayloadPMTK001
{
	typedef Type::tUInt<tUInt8, 0> status_type;

	status_type GPS;
	status_type GLONASS;
	status_type BEIDOU;

	explicit tPayloadPMTK001(const tPayloadCommon::value_type& val)
		:hidden::tPayloadPMTK001(val)
	{
		if (Try(val))
		{
			GPS = status_type(val[3]);
			GLONASS = status_type(val[4]);
			BEIDOU = status_type(val[5]);
		}
	}

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 7 && hidden::tPayloadPMTK001::Try(val);
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data = hidden::tPayloadPMTK001::GetPayload();

		Data.push_back(GPS.ToString());
		Data.push_back(GLONASS.ToString());
		Data.push_back(BEIDOU.ToString());
		Data.push_back("0");

		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadPMTK010
{
	enum class tStatus : tUInt8
	{
		UNKNOWN = 0,
		STARTUP,
		EPO,//Notification for the host aiding Extended Prediction Orbit (EPO)
		NORMAL,//Notification for the transition to Normal mode is successfully done
		UNINITIALIZED = 0xFF,//Notification for the transition to Normal mode is successfully done
	};

	typedef Type::tUInt<tStatus, 3> status_type;

	status_type Status;

	tPayloadPMTK010() = default;
	explicit tPayloadPMTK010(const tPayloadCommon::value_type& val)
	{
		if (Try(val))
		{
			Status = status_type(val[1]);
		}
	}

	static const char* GetID() { return "PMTK010"; }

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 2 && !std::strcmp(val[0].c_str(), GetID());
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GetID());
		Data.push_back(Status.ToString());

		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadPMTK011
{
	typedef std::string message_type;

	message_type Message;

	tPayloadPMTK011() = default;
	explicit tPayloadPMTK011(const tPayloadCommon::value_type& val)
	{
		if (Try(val))
		{
			Message = val[1];
		}
	}

	static const char* GetID() { return "PMTK011"; }

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 2 && !std::strcmp(val[0].c_str(), GetID());
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GetID());
		Data.push_back(Message);

		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace hidden
{

enum class tPayloadPMTK10xResetState : tUInt8
{
	Hot = 1,
	Warm,
	Cold,
	Full
};

template <tPayloadPMTK10xResetState State>
struct tPayloadPMTK10xReset
{
	tPayloadPMTK10xReset() = default;
	explicit tPayloadPMTK10xReset(const tPayloadCommon::value_type& val) {}

	static const char* GetID()
	{
		switch (State)
		{
		case tPayloadPMTK10xResetState::Hot: return "PMTK101";
		case tPayloadPMTK10xResetState::Warm: return "PMTK102";
		case tPayloadPMTK10xResetState::Cold: return "PMTK103";
		case tPayloadPMTK10xResetState::Full: return "PMTK104";
		}

		assert(false);

		return "";
	}

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 1 && !std::strcmp(val[0].c_str(), GetID());
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GetID());

		return Data;
	}
};

}
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef hidden::tPayloadPMTK10xReset<hidden::tPayloadPMTK10xResetState::Hot> tPayloadPMTK101ResetHot;
typedef hidden::tPayloadPMTK10xReset<hidden::tPayloadPMTK10xResetState::Warm> tPayloadPMTK102ResetWarm;
typedef hidden::tPayloadPMTK10xReset<hidden::tPayloadPMTK10xResetState::Cold> tPayloadPMTK103ResetCold;
typedef hidden::tPayloadPMTK10xReset<hidden::tPayloadPMTK10xResetState::Full> tPayloadPMTK104ResetFull;
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadPMTK314
{
	typedef Type::tUInt<tUInt8, 0> status_type;

	status_type GLL;
	status_type RMC;
	status_type VTG;
	status_type GGA;
	status_type GSA;
	status_type GSV;
	status_type ZDA;

	tPayloadPMTK314() = default;
	tPayloadPMTK314(tUInt8 gll, tUInt8 rmc, tUInt8 vtg, tUInt8 gga, tUInt8 gsa, tUInt8 gsv, tUInt8 zda)
		:GLL(gll), RMC(rmc), VTG(vtg), GGA(gga), GSA(gsa), GSV(gsv), ZDA(zda)
	{ }
	explicit tPayloadPMTK314(const tPayloadCommon::value_type& val)
	{
		if (Try(val))
		{
			GLL = status_type(val[1]);
			RMC = status_type(val[2]);
			VTG = status_type(val[3]);
			GGA = status_type(val[4]);
			GSA = status_type(val[5]);
			GSV = status_type(val[6]);
			ZDA = status_type(val[18]);
		}
	}

	static const char* GetID() { return "PMTK314"; }

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 20 && !std::strcmp(val[0].c_str(), GetID());
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GetID());
		Data.push_back(GLL.ToString());
		Data.push_back(RMC.ToString());
		Data.push_back(VTG.ToString());
		Data.push_back(GGA.ToString());
		Data.push_back(GSA.ToString());
		Data.push_back(GSV.ToString());
		Data.push_back("0");
		Data.push_back("0");
		Data.push_back("0");
		Data.push_back("0");
		Data.push_back("0");
		Data.push_back("0");
		Data.push_back("0");
		Data.push_back("0");
		Data.push_back("0");
		Data.push_back("0");
		Data.push_back("0");
		Data.push_back(ZDA.ToString());
		Data.push_back("0");

		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadPMTK353
{
	typedef Type::tUInt<tUInt8, 0> status_type;

	status_type GPS;
	status_type GLONASS;

	tPayloadPMTK353() = default;
	tPayloadPMTK353(bool gps, bool glonass)
		:GPS(gps ? 1 : 0), GLONASS(glonass ? 1 : 0)
	{ }
	explicit tPayloadPMTK353(const tPayloadCommon::value_type& val)
	{
		if (Try(val))
		{
			GPS = status_type(val[1]);
			GLONASS = status_type(val[2]);
		}
	}

	static const char* GetID() { return "PMTK353"; }

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 3 && !std::strcmp(val[0].c_str(), GetID());
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GetID());
		Data.push_back(GPS.ToString());
		Data.push_back(GLONASS.ToString());

		return Data;
	}

	Type::tGNSS_State GetState()
	{
		tUInt8 State = 0;

		if (GPS.Value) State |= 1;
		if (GLONASS.Value) State |= (1 << 1);

		return static_cast<Type::tGNSS_State>(State);
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadPMTK705
{
	typedef std::string release_type;
	typedef std::string build_type;
	typedef std::string internal_use_1_type;
	typedef std::string internal_use_2_type;

	release_type Release;
	build_type Build;
	internal_use_1_type InternalUse1;
	internal_use_2_type InternalUse2;

	tPayloadPMTK705() = default;
	tPayloadPMTK705(const std::string& release, const std::string& revision, const std::string& internalUse1, const std::string& internalUse2)
		:Release(release), Build(revision), InternalUse1(internalUse1), InternalUse2(internalUse2)
	{ }
	explicit tPayloadPMTK705(const tPayloadCommon::value_type& val)
	{
		if (Try(val))
		{
			Release = val[1];
			Build = val[2];
			InternalUse1 = val[3];
			InternalUse2 = val[4];
		}
	}

	static const char* GetID() { return "PMTK705"; }

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 5 && !std::strcmp(val[0].c_str(), GetID());
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GetID());
		Data.push_back(Release);
		Data.push_back(Build);
		Data.push_back(InternalUse1);
		Data.push_back(InternalUse2);

		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
	}
}
