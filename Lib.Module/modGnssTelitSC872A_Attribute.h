///////////////////////////////////////////////////////////////////////////////////////////////////
// modGnssTelitSC872A_Attribute.h
//
// Standard ISO/IEC 114882, C++11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <mutex>
#include <string>

namespace mod
{

class tGnssTelitSC872ADataSet
{
	mutable std::mutex m_Mtx;

	mutable bool m_Changed = false;

	std::string m_DataValue1;

public:
	bool Changed();

	std::string GetDataValue1() const;

	void SetDataValue1(const std::string& value);
};

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
