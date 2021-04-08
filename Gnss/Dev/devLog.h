///////////////////////////////////////////////////////////////////////////////////////////////////
// devLog.h
//
// Standard ISO/IEC 114882, C++14
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 01 15  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <utilsLog.h>

#include <cstdint>

namespace dev
{

class tLog : public utils::tLog
{
public:
	enum class tID : std::uint32_t
	{
		NoID = 0,
		Test_0 = (1 << 0),
		Test_1 = (1 << 1),
		GNSS = (1 << 2),
	};

	union tSettings
	{
		struct
		{
			std::uint32_t Log_0 : 1;
			std::uint32_t Log_1 : 1;
			std::uint32_t GNSS : 1;

			std::uint32_t : 29;
		}Field;

		std::uint32_t Value = 0;
	};

	static tSettings LogSettings;

private:
	tID m_ID = tID::NoID;
	const char* m_Sign = nullptr;

	tLog() = delete;

public:
	explicit tLog(dev::tLog::tID id, const char* sign);

protected:
	const char* GetSign() const override;

	void WriteLog(const std::string& msg) override;
};

}
