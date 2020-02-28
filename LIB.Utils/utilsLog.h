///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsLog.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |    long ago   |
// |      2     |   2016 05 16  | std::string instead of ssystem::String
// |      3     |   2016 05 30  | Added WriteHex(std::vector<char> msg, bool timestamp = true)
// |            |               | Added WriteHex(std::string msg, std::vector<char>& data, bool timestamp)
// |      4     |   2016 06 03  | Added WriteLog(std::string msg, bool timestamp, bool error);
// |            |               | Added void WriteInfoBlock(std::string msg)
// |            |               | Corrected WriteHex(std::string msg, std::vector<char>& data, bool timestamp)
// |      5     |   2016 06 29  | Slightly modified output
// |      6     |   2016 07 06  | Added virtual void WriteLineArg(bool timestamp, const char* format, ...)
// |      7     |   2016 07 27  | Corrected void tLog::Write(std::string msg, bool timestamp)
// |      8     |   2016 09 16  | Added virtual void WriteInfoLine(std::string msg, bool timestamp = true);
// |      9     |   2016 11 15  | Corrected void tLog::WriteHex(std::string msg, std::vector<char>& data, bool timestamp)
// |     10     |   2017 03 06  | Added LIB_UTILS_LOG
// |     11     |   2017 03 29  | Introduced namespace utils::log
// |     12     |   2017 04 07  | Introduced LIB_UTILS_LOG_ENDL
// |     13     |   2017 10 29  | Added WriteInfoLineArg(bool timestamp, std::string msg)
// |     14     |   2018 02 10  | Added support of colours
// |     15     |   2018 09 27  | Added Write(std::string msg, tLogColour textColour, bool timestamp = true)
// |            |               | Added WriteLine(std::string msg, tLogColour textColour, bool timestamp = true)
// |     16     |   2019 04 25  | Refactored
// |     17     |   2019 05 11  | Updated
// |     18     |   2020 01 17  | Refactored, thread safe
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include <utilsBase.h>

#include <cstdarg>
#include <string>
#include <mutex>

namespace utils
{

enum class tLogColour : unsigned char
{
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	White,
	Default,
	LightGray,
	LightRed,
	LightGreen,
	LightYellow,
	LightBlue,
	LightMagenta,
	LightCyan,
	LightWhite,
};

#ifdef LIB_UTILS_LOG

class tLog
{
	mutable std::mutex m_Mtx;

	bool m_ColourEnabled = false;

public:
	tLog() = delete;
	explicit tLog(bool colourEnabled = false);
	virtual ~tLog() { }

	void ColourEnabled(bool state);

	void Write(bool timestamp, tLogColour textColour, const std::string& msg);
	void Write(bool timestamp, tLogColour textColour, const char* format, ...);

	void WriteLine();
	void WriteLine(bool timestamp, tLogColour textColour, const std::string& msg);
	void WriteLine(bool timestamp, tLogColour textColour, const char* format, ...);

	void WriteHex(bool timestamp, tLogColour textColour, const std::string& msg, const tVectorUInt8& data);

protected:
	virtual void WriteLog(const std::string& msg) = 0;

private:
	virtual void WriteLog(bool timestamp, bool endl, tLogColour textColour, const std::string& msg);
};

#else//LIB_UTILS_LOG

class tLog
{
public:
	tLog() = delete;
	explicit tLog(bool colourEnabled = false) { }
	virtual ~tLog() { }

	void ColourEnabled(bool state) { }

	void Write(bool timestamp, tLogColour textColour, const std::string& msg) { }
	void Write(bool timestamp, tLogColour textColour, const char* format, ...) { }

	void WriteLine() { }
	void WriteLine(bool timestamp, tLogColour textColour, const std::string& msg) { }
	void WriteLine(bool timestamp, tLogColour textColour, const char* format, ...) { }

	void WriteHex(bool timestamp, tLogColour textColour, const std::string& msg, const tVectorUInt8& data) { }

protected:
	virtual void WriteLog(const std::string& msg) = 0;
};

#endif//LIB_UTILS_LOG

}
