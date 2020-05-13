#include "devDB.h"
#include "devSettings.h"

#include <memory>//[TEST]
#include <mutex>

#include <iomanip>
#include <sstream>
#include <utility>

#include <cstdio>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace dev
{
	namespace db
	{

struct tMYSQL
{
	mutable std::recursive_mutex Mtx;
	MYSQL MySQL{};
	std::string DB;

	utils::tUInt8 UpdateID = 0xFF;
	utils::tUInt8 RcvID = 0xFF;
}g_MySQL;

typedef std::lock_guard<std::recursive_mutex> tLockGuard;

typedef std::pair<std::string, std::string> tSQLQueryParamPair;
typedef std::vector<tSQLQueryParamPair> tSQLQueryParam;

my_ulonglong Insert(const std::string& table, const tSQLQueryParam& prm, int& cerr);

#define DB_TEST
#ifdef DB_TEST
static void show_mysql_error(MYSQL* mysql)
{
	printf("Error(%d) [%s] \"%s\"", mysql_errno(mysql),
		mysql_sqlstate(mysql),
		mysql_error(mysql));
	exit(-1);
}

static void show_stmt_error(MYSQL_STMT* stmt)
{
	printf("Error(%d) [%s] \"%s\"", mysql_stmt_errno(stmt),
		mysql_stmt_sqlstate(stmt),
		mysql_stmt_error(stmt));
	exit(-1);
}
#endif//DB_TEST

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
			"CREATE TABLE rcv (rcv_id INT(2) NOT NULL AUTO_INCREMENT, timestamp DATETIME NOT NULL, model VARCHAR(50) NOT NULL DEFAULT '', id VARCHAR(50) NOT NULL DEFAULT '', PRIMARY KEY(rcv_id), UNIQUE INDEX(id));",
			"CREATE TABLE pos (pos_id INT(10) NOT NULL AUTO_INCREMENT, timestamp DATETIME NOT NULL, gnss INT(2), date_time DATETIME, valid BOOLEAN, latitude DOUBLE, longitude DOUBLE, altitude DOUBLE, speed FLOAT, course FLOAT, rcv_id INT(2) NOT NULL, update_id INT(2) NOT NULL, PRIMARY KEY(pos_id), INDEX(timestamp));",
			"CREATE TABLE pos_sat (pos_id INT(10) NOT NULL, sat_id INT(3) NOT NULL, elevation INT(2), azimuth INT(3), snr INT(2), PRIMARY KEY(pos_id, sat_id));",
			"CREATE TABLE sat (sat_id INT(3) NOT NULL, gnss VARCHAR(50) NOT NULL DEFAULT '', description VARCHAR(50) NOT NULL DEFAULT '', PRIMARY KEY(sat_id));",
		};

		for (auto& i : ReqList)
		{
			if (!cerr)
			{
				mysql_query(&g_MySQL.MySQL, i.c_str());

				cerr = mysql_errno(&g_MySQL.MySQL);
			}
		}

		if (!cerr)
		{
			try
			{
				boost::property_tree::ptree PTree;
				boost::property_tree::xml_parser::read_xml(g_Settings.GetConfigFileName(), PTree);

				if (PTree.size() > 0)
				{
					auto Root = PTree.begin();

					if (Root->first == "App")
					{
						for (auto a : Root->second)
						{
							if (a.first == "DB_Init")
							{
								for (auto b : a.second)
								{
									if (b.first == "Table")
									{
										std::string Table;

										for (auto c : b.second)
										{
											if (c.first == "<xmlattr>")
											{
												Table = c.second.get<std::string>("ID");
											}
											else if (c.first == "Row")
											{
												if (Table == "sat")
												{
													for (auto d : c.second)
													{
														if (d.first == "<xmlattr>")
														{
															std::string Sat_ID = d.second.get<std::string>("ID");
															std::string GNSS = d.second.get<std::string>("GNSS");
															std::string Descript = d.second.get<std::string>("Description");

															const tSQLQueryParam Query
															{
																{"sat_id", Sat_ID},
																{"gnss", GNSS},
																{"description", Descript},
															};

															Insert("sat", Query, cerr);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			catch (std::exception& e)
			{
				return;
			}
		}
	}
}

bool Open(int& cerr)
{
	if (!cerr)
	{
		tLockGuard Lock(g_MySQL.Mtx);

		if (!cerr && Init(g_Settings.DB.Host, g_Settings.DB.User, g_Settings.DB.Passwd, g_Settings.DB.DB, g_Settings.DB.Port))
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

			if (TableSys.size() == 1 && TableSys.front().size() == 2 && TableSys.front()[1] == DEV_DB_VERSION)
			{
				g_MySQL.UpdateID = utils::Read<utils::tUInt8>(TableSys.front()[0].cbegin(), TableSys.front()[0].cend(), utils::tRadix_10);
			}
			else
			{
				return false;
			}
			
			for (int i = 0; i < 2; ++i)//if the receiver is not on the list - insert it and repeate the search
			{
				tTable TableRCV = GetTableRcv(cerr);

				for (tTableRow& row : TableRCV)
				{
					if (row.size() != 4)
					{
						return false;
					}

					if (row[2] == g_Settings.Main.Model && row[3] == g_Settings.Main.ID)
					{
						g_MySQL.RcvID = utils::Read<utils::tUInt8>(row[0].cbegin(), row[0].cend());
						return true;
					}
				}

				dev::db::InsertTableRcv(cerr);
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

	cerr = mysql_real_query(&g_MySQL.MySQL, Str, static_cast<unsigned long>(std::strlen(Str)));//Contrary to the mysql_query() function, mysql_real_query is binary safe.

	return mysql_insert_id(&g_MySQL.MySQL);
}

my_ulonglong InsertTablePos(const std::string& timestamp, char gnss, const std::string& dateTime, bool valid, double latitude, double longitude, double altitude, double speed, double course, int& cerr)
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
		{"update_id", ToString(g_MySQL.UpdateID)},
	};

	return Insert("pos", Query, cerr);
}

//my_ulonglong InsertTablePosSat(int pos_id, int sat_id, int elevation, int azimuth, int snr, int& cerr)
//{
//	const tSQLQueryParam Query
//	{
//		{"pos_id", ToString(pos_id)},
//		{"sat_id", ToString(sat_id)},
//		{"elevation", ToString(elevation)},
//		{"azimuth", ToString(azimuth)},
//		{"snr", ToString(snr)},
//	};
//
//	return Insert("pos_sat", Query, cerr);
//}

void InsertTablePosSatBulk(tTableSatBulk& table, int& cerr)
{
	tLockGuard Lock(g_MySQL.Mtx);

	MYSQL_STMT* Stmt = mysql_stmt_init(&g_MySQL.MySQL);

	const unsigned int ColumnQty = 5;

	cerr = mysql_stmt_prepare(Stmt, "INSERT INTO pos_sat VALUES (?,?,?,?,?)", -1);//[TBD] - columns

	if (cerr)
		return;

	const std::size_t RowSize = sizeof(tTableSatBulkRow);

	std::unique_ptr<MYSQL_BIND> Bind{ new MYSQL_BIND[ColumnQty] };

	auto InsertBulk = [=, &table, &cerr](MYSQL_BIND* bind, std::size_t& count)->void
	{
		if (cerr || count >= table.size())
			return;

		std::size_t ArraySize = table.size();
		//std::size_t ArraySize = table.size() - count;
		//if (ArraySize > 16)
		//{
		//	ArraySize = 16;
		//}

		memset(bind, 0, sizeof(MYSQL_BIND) * ColumnQty);

		bind[0].buffer = &table[count].pos_id;
		bind[0].buffer_type = MYSQL_TYPE_LONG;
		bind[0].u.indicator = &table[count].pos_id_ind;

		bind[1].buffer = &table[count].sat_id;
		bind[1].buffer_type = MYSQL_TYPE_LONG;
		bind[1].u.indicator = &table[count].sat_id_ind;

		bind[2].buffer = &table[count].elevation;
		bind[2].buffer_type = MYSQL_TYPE_LONG;
		bind[2].u.indicator = &table[count].elevation_ind;

		bind[3].buffer = &table[count].azimuth;
		bind[3].buffer_type = MYSQL_TYPE_LONG;
		bind[3].u.indicator = &table[count].azimuth_ind;

		bind[4].buffer = &table[count].snr;
		bind[4].buffer_type = MYSQL_TYPE_LONG;
		bind[4].u.indicator = &table[count].snr_ind;

		mysql_stmt_attr_set(Stmt, STMT_ATTR_ARRAY_SIZE, &ArraySize);
		mysql_stmt_attr_set(Stmt, STMT_ATTR_ROW_SIZE, &RowSize);

		mysql_stmt_bind_param(Stmt, bind);

		cerr = mysql_stmt_execute(Stmt);

		count += ArraySize;
	};

	std::size_t Count = 0;
	while (!cerr && Count < table.size())
	{
		InsertBulk(Bind.get(), Count);
	}

#ifdef DB_TEST
	if (cerr)
		show_stmt_error(Stmt);
#endif//DB_TEST

	mysql_stmt_close(Stmt);
}

my_ulonglong InsertTableRcv(int& cerr)
{
	std::string Timestamp = GetTimestamp(std::time(nullptr));

	const tSQLQueryParam Query
	{
		{"timestamp", Timestamp},
		{"model", g_Settings.Main.Model},
		{"id", g_Settings.Main.ID},
	};

	return Insert("rcv", Query, cerr);
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