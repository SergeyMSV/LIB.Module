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
	ConfigFileName = fileName;

	boost::property_tree::ptree PTree;
	boost::property_tree::xml_parser::read_xml(ConfigFileName, PTree);

	if (auto Value = PTree.get_child_optional("App.Settings.DB"))
	{
		auto ValueIter = (*Value).begin();

		if (ValueIter->first == "<xmlattr>")
		{
			DB.Host = ValueIter->second.get<std::string>("Host");
			DB.User = ValueIter->second.get<std::string>("User");
			DB.Passwd = ValueIter->second.get<std::string>("Passwd");
			DB.DB = ValueIter->second.get<std::string>("Name");
			DB.Port = ValueIter->second.get<unsigned int>("Port");
		}
	}

	if (auto Value = PTree.get_child_optional("App.Settings.GNSS_Receiver"))
	{
		auto ValueIter = (*Value).begin();

		if (ValueIter->first == "<xmlattr>")
		{
			Main.Model = ValueIter->second.get<std::string>("Model");
			Main.ID = ValueIter->second.get<std::string>("ID");
		}
	}

	if (auto Value = PTree.get_child_optional("App.Settings.SerialPort"))
	{
		auto ValueIter = (*Value).begin();

		if (ValueIter->first == "<xmlattr>")
		{
			SerialPort.ID = ValueIter->second.get<std::string>("ID");
			SerialPort.BR = ValueIter->second.get<utils::tUInt32>("BR");
		}
	}
}

mod::tGnssTaskScript tSettings::GetTaskScript(const std::string& id)
{
	boost::property_tree::ptree PTree;
	boost::property_tree::xml_parser::read_xml(ConfigFileName, PTree);

	mod::tGnssTaskScript Script;

	if (auto Value = PTree.get_child_optional("App.Settings.GNSS_Receiver.TaskScript." + id))
	{
		for (auto i : *Value)
		{
			if (i.first == "Cmd")
			{
				if (auto Attr = i.second.get_child_optional("<xmlattr>"))
				{
					mod::tGnssTaskScriptCmd Cmd;
					Cmd.Msg = Attr->get<std::string>("Msg");
					Cmd.RspHead = Attr->get<std::string>("RspHead");
					Cmd.RspBody = Attr->get<std::string>("RspBody");
					Cmd.CaseRspWrong = Attr->get<std::string>("CaseRspWrong");
					Cmd.RspWait_us = Attr->get<decltype(Cmd.RspWait_us)>("RspWait_us");

					Cmd.TimePause_us = Attr->get<decltype(Cmd.TimePause_us)>("TimePause_us");
					Script.push_back(Cmd);
				}
			}
		}
	}

	return Script;
}

}