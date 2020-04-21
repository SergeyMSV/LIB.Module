#include "devDB.h"

#include <mutex>

#include <iomanip>
#include <sstream>
#include <utility>

#include <cstdio>

namespace dev
{
	namespace db
	{

struct tMYSQL
{
	mutable std::recursive_mutex Mtx;
	MYSQL MySQL{};
	std::string DB;

	utils::tUInt8 RcvID = 0xFF;
}g_MySQL;

typedef std::lock_guard<std::recursive_mutex> tLockGuard;

typedef std::pair<std::string, std::string> tSQLQueryParamPair;
typedef std::vector<tSQLQueryParamPair> tSQLQueryParam;

template<typename T>
std::string ToString(T value)
{
	std::stringstream Stream;

	if (std::is_floating_point<T>::value)
	{
		Stream << value;
	}
	else//std::is_integral<T>::value
	{
		Stream << (unsigned int)value;
	}

	return Stream.str();
}

std::string ToString(const std::tm& time)
{
	std::stringstream Stream;
	Stream << std::put_time(&time, "%F %T");
	return Stream.str();
}

std::string ToString(const std::time_t& timestamp)
{
	return ToString(*std::gmtime(&timestamp));
}

bool Init(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, unsigned int port)
{
	tLockGuard Lock(g_MySQL.Mtx);

	g_MySQL.DB = db;

	mysql_init(&g_MySQL.MySQL);

	mysql_real_connect(&g_MySQL.MySQL, host.c_str(), user.c_str(), passwd.c_str(), "", port, "", 0);//DB is not specified intentionally

	return mysql_errno(&g_MySQL.MySQL) == 0;
}

void Create(int& cerr)
{
	tLockGuard Lock(g_MySQL.Mtx);

	if (!cerr && mysql_errno(&g_MySQL.MySQL) == ER_BAD_DB_ERROR)
	{
		cerr = mysql_query(&g_MySQL.MySQL, std::string("CREATE DATABASE IF NOT EXISTS " + g_MySQL.DB).c_str());

		if (!cerr)
		{
			cerr = mysql_query(&g_MySQL.MySQL, std::string("USE " + g_MySQL.DB).c_str());
		}

		const std::vector<std::string> ReqList
		{
			"CREATE TABLE sys (update_id INT(2) NOT NULL AUTO_INCREMENT, version VARCHAR(20) NOT NULL DEFAULT '', PRIMARY KEY(update_id));",
			"INSERT INTO sys (version) VALUE('" DEV_DB_VERSION "');",
			"CREATE TABLE rcv (rcv_id INT(2) NOT NULL AUTO_INCREMENT, timestamp DATETIME NOT NULL, model VARCHAR(50) NOT NULL DEFAULT '', ident VARCHAR(50) NOT NULL DEFAULT '', PRIMARY KEY(rcv_id), UNIQUE INDEX(ident));",
			"CREATE TABLE pos (pos_id INT(10) NOT NULL AUTO_INCREMENT, timestamp DATETIME NOT NULL, gnss INT(2), date_time DATETIME, valid BOOLEAN, latitude DOUBLE, longitude DOUBLE, altitude DOUBLE, speed FLOAT, course FLOAT, rcv_id INT(2) NOT NULL, update_id INT(2) NOT NULL, PRIMARY KEY(pos_id), INDEX(timestamp));",
			"CREATE TABLE sat (pos_id INT(10) NOT NULL, sat_id INT(2) NOT NULL, elevation INT(2), azimuth INT(3), snr INT(2), PRIMARY KEY(pos_id, sat_id), INDEX(pos_id, sat_id));",
		};

		for (auto& i : ReqList)
		{
			if (!cerr)
			{
				mysql_query(&g_MySQL.MySQL, i.c_str());

				cerr = mysql_errno(&g_MySQL.MySQL);
			}
		}
	}
}

bool Open(int& cerr)
{
	if (!cerr)
	{
		tLockGuard Lock(g_MySQL.Mtx);

		if (!cerr && Init(g_ConfigINI.DB.Host, g_ConfigINI.DB.User, g_ConfigINI.DB.Passwd, g_ConfigINI.DB.DB, g_ConfigINI.DB.Port))
		{
			cerr = mysql_query(&g_MySQL.MySQL, std::string("USE " + g_MySQL.DB).c_str());
		}

		if (cerr)
		{
			cerr = 0;
			Create(cerr);
		}

		if (!cerr)
		{
			tTable TableSys = GetTableSys(cerr);

			if (TableSys.size() != 1 || TableSys.front().size() != 2 || TableSys.front()[1] != DEV_DB_VERSION)
			{
				return false;
			}
			
			for (int i = 0; i < 2; ++i)//if there is no this receiver on the list - insert it and repeate the search
			{
				tTable TableRCV = GetTableRcv(cerr);

				for (tTableRow& row : TableRCV)
				{
					if (TableRCV.size() != 4)
					{
						return false;
					}

					for (std::string& i : row)
					{
						if (i == g_ConfigINI.Main.Ident)
						{
							g_MySQL.RcvID = utils::Read<utils::tUInt8>(row[0].begin(), row[0].end());
							return true;
						}
					}

					dev::db::InsertTableRcv(cerr);
				}
			}
		}
	}
	return false;
}

bool Open()
{
	int Cerr = 0;
	return Open(Cerr);
}

my_ulonglong Insert(const std::string& table, const tSQLQueryParam& prm, int& cerr)
{
	if (cerr)
		return 0;

	char Str[256]{};

	std::string StrPrm;
	std::string StrValue;

	for (std::size_t i = 0; i < prm.size(); ++i)
	{
		StrPrm += prm[i].first;
		StrValue += "'" + prm[i].second + "'";

		if (i < prm.size() - 1)
		{
			StrPrm += ',';
			StrValue += ',';
		}
	}

	std::sprintf(Str, "INSERT INTO %s (%s) VALUE(%s);", table.c_str(), StrPrm.c_str(), StrValue.c_str());

	tLockGuard Lock(g_MySQL.Mtx);

	cerr = mysql_real_query(&g_MySQL.MySQL, Str, std::strlen(Str));//Contrary to the mysql_query() function, mysql_real_query is binary safe.

	return mysql_insert_id(&g_MySQL.MySQL);
}

my_ulonglong InsertTableRcv(int& cerr)
{
	std::string Timestamp = GetTimestamp(std::time(nullptr));

	const tSQLQueryParam Query
	{
		{"timestamp", Timestamp},
		{"model", g_ConfigINI.Main.Model},
		{"ident", g_ConfigINI.Main.Ident},
	};

	return Insert("rcv", Query, cerr);
}

my_ulonglong InsertTablePos(const std::string& timestamp, char gnss, const std::string& dateTime, bool valid, double latitude, double longitude, double altitude, double speed, double course, unsigned char update_id, int& cerr)
{
	std::string RcvID = ToString(g_MySQL.RcvID);

	const tSQLQueryParam Query
	{
		{"timestamp", timestamp},
		{"gnss", ToString(gnss)},
		{"date_time", dateTime},
		{"valid", ToString(valid)},
		{"latitude", ToString(latitude)},
		{"longitude", ToString(longitude)},
		{"altitude", ToString(altitude)},
		{"speed", ToString(speed)},
		{"course", ToString(course)},
		{"rcv_id", RcvID},
		{"update_id", ToString(update_id)},
	};

	return Insert("pos", Query, cerr);
}

my_ulonglong InsertTableSat(int pos_id, int sat_id, int elevation, int azimuth, int snr, int& cerr)
{
	const tSQLQueryParam Query
	{
		{"pos_id", ToString(pos_id)},
		{"sat_id", ToString(sat_id)},
		{"elevation", ToString(elevation)},
		{"azimuth", ToString(azimuth)},
		{"snr", ToString(snr)},
	};

	return Insert("sat", Query, cerr);
}

tTable GetTable(std::string table, int& cerr)
{
	if (!cerr)
	{
		tTable List;

		tLockGuard Lock(g_MySQL.Mtx);

		std::string Query("SELECT * FROM ");
		Query += table + ';';

		cerr = mysql_query(&g_MySQL.MySQL, Query.c_str());

		MYSQL_RES* Res = mysql_use_result(&g_MySQL.MySQL);

		unsigned int Count = mysql_num_fields(Res);

		while (true)//[#] true or [limit]
		{
			tTableRow Row;

			MYSQL_ROW AB = mysql_fetch_row(Res);

			if (AB == nullptr)
				break;

			for (unsigned int i = 0; i < Count; ++i)
			{
				Row.push_back(AB[i]);
			}

			List.push_back(std::forward<tTableRow>(Row));
		}

		mysql_free_result(Res);

		return List;
	}

	return tTable();
}

tTable GetTablePos(int& cerr)
{
	return GetTable("pos", cerr);
}

tTable GetTableRcv(int& cerr)
{
	return GetTable("rcv", cerr);
}

tTable GetTableSat(int& cerr)
{
	return GetTable("sat", cerr);
}

tTable GetTableSys(int& cerr)
{
	return GetTable("sys", cerr);
}

void Clear(int& cerr)
{
	Close();
	Drop(cerr);
	Open(cerr);
}

bool Clear()
{
	int Cerr = 0;
	Clear(Cerr);
	return Cerr == 0;
}

void Drop(int& cerr)
{
	if (!cerr)
	{
		tLockGuard Lock(g_MySQL.Mtx);

		cerr = mysql_query(&g_MySQL.MySQL, std::string("DROP DATABASE IF EXISTS " + g_MySQL.DB).c_str());
	}
}

bool Drop()
{
	int Cerr = 0;
	Drop(Cerr);
	return Cerr == 0;
}

unsigned int GetERR()
{
	tLockGuard Lock(g_MySQL.Mtx);

	return mysql_errno(&g_MySQL.MySQL);
}

void Close()
{
	tLockGuard Lock(g_MySQL.Mtx);

	mysql_close(&g_MySQL.MySQL);
}

std::string GetTimestamp(const std::time_t& timestamp)
{
	return ToString(timestamp);
}

std::string GetTimestamp(unsigned char year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second)
{
	std::tm Time{};
	Time.tm_year = year + 100;
	Time.tm_mon = month - 1;
	Time.tm_mday = day;
	Time.tm_hour = hour;
	Time.tm_min = minute;
	Time.tm_sec = second;

	return ToString(Time);
}

std::string GetTimestampNow()
{
	return GetTimestamp(std::time(nullptr));
}

	}
}


/*
	//Load data...

		std::string Timestamp = GetTimestamp(std::time(nullptr));

		Insert(&MySQL, Timestamp, "Blablabla 11", "super new1", Cerr);
		Insert(&MySQL, Timestamp, "Blablabla 11", "super new2", Cerr);
		Insert(&MySQL, Timestamp, "Blablabla 12", "super new3", Cerr);
		Insert(&MySQL, Timestamp, "Blablabla 14", "super new4", Cerr);
		Insert(&MySQL, Timestamp, "Blablabla 11", "super new5", Cerr);

		Insert(&MySQL, Timestamp, 2, "2020-01-28 14.23:51", true, 55.123456, 37.123456, 234.546344, 345.219, 123.345, 2, 1, Cerr);
		Insert(&MySQL, Timestamp, 2, "2020-01-28 14.23:52", true, 55.123456, 37.123456, 234.546344, 345.219, 123.345, 2, 1, Cerr);
		Insert(&MySQL, Timestamp, 2, "2020-01-28 14.23:53", true, 55.123456, 37.123456, 234.546344, 345.219, 123.345, 2, 1, Cerr);
		Insert(&MySQL, Timestamp, 1, "2020-01-28 14.23:54", true, 55.123456, 37.123456, 234.546344, 345.219, 123.345, 2, 1, Cerr);
		Insert(&MySQL, Timestamp, 1, "2020-01-28 14.23:55", true, 55.123456, 37.123456, 234.546344, 345.219, 123.345, 2, 1, Cerr);

		Insert(&MySQL, 3, 31, 12, 345, 91, Cerr);
		Insert(&MySQL, 3, 32, 12, 345, 91, Cerr);
		Insert(&MySQL, 3, 33, 12, 345, 91, Cerr);
		Insert(&MySQL, 3, 34, 12, 345, 91, Cerr);
		Insert(&MySQL, 3, 35, 12, 345, 91, Cerr);
		Insert(&MySQL, 5, 31, 13, 345, 91, Cerr);
		Insert(&MySQL, 5, 32, 13, 345, 91, Cerr);
		Insert(&MySQL, 5, 33, 13, 345, 91, Cerr);
		Insert(&MySQL, 5, 34, 13, 345, 91, Cerr);
		Insert(&MySQL, 5, 35, 13, 345, 91, Cerr);

		if (Cerr)
		{
			std::cout << "ERR DB: " << mysql_errno(&MySQL) << '\n';
		}
*/