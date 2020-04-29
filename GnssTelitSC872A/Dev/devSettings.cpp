#include "devSettings.h"

//#include <memory>//[TEST]
//#include <mutex>

//#include <iomanip>
//#include <sstream>
//#include <utility>

//#include <cstdio>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace dev
{

tSettings g_Settings;

tSettings::tSettings(const std::string& fileName)
{
	boost::property_tree::ptree PTree;
	boost::property_tree::xml_parser::read_xml(fileName, PTree);

	if (PTree.size() > 0)
	{
		auto Root = PTree.begin();

		if (Root->first == "App")
		{
			for (auto a : Root->second)
			{
				if (a.first == "Settings")
				{
					for (auto b : a.second)
					{
						if (b.first == "DB")
						{
							for (auto c : b.second)
							{
								if (c.first == "<xmlattr>")
								{
									DB.Host = c.second.get<std::string>("Host");
									DB.User = c.second.get<std::string>("User");
									DB.Passwd = c.second.get<std::string>("Passwd");
									DB.DB = c.second.get<std::string>("Name");
									DB.Port = c.second.get<unsigned int>("Port");
								}
							}
						}
						else if (b.first == "GNSS_Receiver")
						{
							for (auto c : b.second)
							{
								if (c.first == "<xmlattr>")
								{
									Main.Model = c.second.get<std::string>("Model");
									Main.ID = c.second.get<std::string>("ID");
								}
								else
								{

								}
							}
						}
						else if (b.first == "SerialPort")
						{
							for (auto c : b.second)
							{
								if (c.first == "<xmlattr>")
								{
									SerialPort.ID = c.second.get<std::string>("ID");
									SerialPort.BR = c.second.get<utils::tUInt32>("BR");
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