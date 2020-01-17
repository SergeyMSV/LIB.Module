#include "utilsLog.h"

#include <chrono>
#include <cstdio>
#include <ctime>

#ifdef LIB_UTILS_LOG

namespace utils
{

void tLog::Write(bool timestamp, tLogColour textColour, const std::string& msg)
{
	WriteLog(timestamp, false, textColour, msg);
}

void tLog::Write(bool timestamp, tLogColour textColour, const char* format, ...)
{
	char Buffer[160];

	std::va_list Args;

	va_start(Args, format);

	std::vsprintf(Buffer, format, Args);

	std::string Str(Buffer);

	va_end(Args);

	WriteLog(timestamp, false, textColour, Str);
}

void tLog::WriteLine()
{
	WriteLog(false, true, tLogColour::Default, "");
}

void tLog::WriteLine(bool timestamp, tLogColour textColour, const std::string& msg)
{
	WriteLog(timestamp, true, textColour, msg);
}

void tLog::WriteLine(bool timestamp, tLogColour textColour, const char* format, ...)
{
	char Buffer[160];

	std::va_list Args;

	va_start(Args, format);

	std::vsprintf(Buffer, format, Args);

	std::string Str(Buffer);

	va_end(Args);

	WriteLog(timestamp, true, textColour, Str);
}

void tLog::WriteHex(bool timestamp, tLogColour textColour, const std::string& msg, const tVectorUInt8& data)
{
	std::string Str = msg + '\n';

	char Figure[5];

	std::string Substr;

	for (std::size_t i = 0; i < data.size(); ++i)
	{
		std::sprintf(Figure, "%02X ", data[i]);
		Str += Figure;

		if (data[i] <= 0x20 || data[i] == 0x25)
		{
			Substr += '.';
		}
		else
		{
			Substr += data[i];
		}

		if (((i + 1) % 16) == 0)
		{
			Str += "  " + Substr;

			Substr.clear();

			if (i < data.size() - 1)//It's not needed fot the last string
			{
				Str += '\n';
			}
		}
		else if (((i + 1) % 8) == 0)
		{
			Str += "  ";
		}
	}

	std::size_t FullStringsEnd = Str.find_last_of('\n');

	std::size_t Filled = Str.size() - FullStringsEnd;

	const std::size_t StrHexPratLineSize = 53;

	if (Filled <= StrHexPratLineSize)
	{
		std::size_t Empty = StrHexPratLineSize - Filled;

		for (std::size_t i = 0; i < Empty; ++i)
		{
			Str += ' ';
		}

		Str += Substr;
	}

	WriteLog(timestamp, true, textColour, Str);
}

void tLog::WriteLog(bool timestamp, bool endl, tLogColour textColour, const std::string& msg)
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	if (timestamp)
	{
		auto TimeNow = std::chrono::high_resolution_clock::now();

		std::chrono::duration<unsigned long, std::milli> Time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(TimeNow.time_since_epoch());

		unsigned long Tm_ms = Time_ms.count() % 1000;

		std::chrono::system_clock::time_point TimeSystemNow = std::chrono::system_clock::now();

		std::time_t Time = std::chrono::system_clock::to_time_t(TimeSystemNow);

		tm* TM = std::localtime(&Time);

		char Str[20];

		std::sprintf(Str, "[%.2d.%.2d:%.3d]", static_cast<unsigned char>(TM->tm_min), static_cast<unsigned char>(TM->tm_sec), static_cast<unsigned char>(Tm_ms));

		WriteLog(Str);
	}

#ifdef LIB_UTILS_LOG_COLOUR

	std::string Str = "\x1b[";

	switch (textColour)
	{
	case tLogColour::Black: Str += "30"; break;
	case tLogColour::Red: Str += "31"; break;
	case tLogColour::Green: Str += "32"; break;
	case tLogColour::Yellow: Str += "33"; break;
	case tLogColour::Blue: Str += "34"; break;
	case tLogColour::Magenta: Str += "35"; break;
	case tLogColour::Cyan: Str += "36"; break;
	case tLogColour::White: Str += "37"; break;
	case tLogColour::Default: Str += "39"; break;
	case tLogColour::LightGray: Str += "90"; break;
	case tLogColour::LightRed: Str += "91"; break;
	case tLogColour::LightGreen: Str += "92"; break;
	case tLogColour::LightYellow: Str += "93"; break;
	case tLogColour::LightBlue: Str += "94"; break;
	case tLogColour::LightMagenta: Str += "95"; break;
	case tLogColour::LightCyan: Str += "96"; break;
	case tLogColour::LightWhite: Str += "97"; break;
	default: Str += "39"; break;
	}

	Str += "m" + msg + "\x1b[0m";

	if (endl)
	{
		Str += '\n';
	}

	WriteLog(Str);

#else//LIB_UTILS_LOG_COLOUR
	if (endl)
	{
		WriteLog(msg + '\n');
	}
	else
	{
		WriteLog(msg);
	}
#endif//LIB_UTILS_LOG_COLOUR
}

}

#endif//LIB_UTILS_LOG
