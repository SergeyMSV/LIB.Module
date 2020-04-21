///////////////////////////////////////////////////////////////////////////////////////////////////
// devDB.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 03 31  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>

#include <mysql.h>
#include <mysqld_error.h>

#include <list>
#include <string>
#include <vector>
#include <utility>

#include <ctime>

namespace dev
{
	namespace db
	{

//bool Init(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, unsigned int port);
//
////void DB_Create(int& cerr);
////bool DB_Create();

bool Open(int& cerr);
bool Open();

my_ulonglong InsertTablePos(const std::string& timestamp, char gnss, const std::string& dateTime, bool valid, double latitude, double longitude, double altitude, double speed, double course, unsigned char update_id, int& cerr);
my_ulonglong InsertTableRcv(int& cerr);
my_ulonglong InsertTableSat(int pos_id, int sat_id, int elevation, int azimuth, int snr, int& cerr);

typedef std::vector<std::string> tTableRow;
typedef std::list<tTableRow> tTable;

tTable GetTablePos(int& cerr);
tTable GetTableRcv(int& cerr);
tTable GetTableSat(int& cerr);
tTable GetTableSys(int& cerr);

void Clear(int& cerr);
bool Clear();

void Drop(int& cerr);
bool Drop();

unsigned int GetERR();

void Close();

std::string GetTimestamp(const std::time_t& timestamp);
std::string GetTimestamp(unsigned char year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second);
std::string GetTimestampNow();

	}
}