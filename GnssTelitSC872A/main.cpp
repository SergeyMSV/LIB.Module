#include <devConfig.h>

#include <devDB.h>
#include <devGNSS.h>
//#include <devShell.h>

#include <atomic>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

tConfigINI g_ConfigINI;

namespace dev
{
	void ThreadFunShell();
}

tDataSetMainControl g_DataSetMainControl;

void Thread_GNSS_Handler(std::promise<std::string>& promise)
{
	dev::tLog Log(dev::tLog::LogId_GNSS);

	//Log.LogSettings.Field.GNSS = 1;

	boost::asio::io_context IO;

	try
	{
		dev::tGNSS Dev(&Log, IO);

		std::thread Thread_IO([&]() { IO.run(); });
		std::thread Thread_Dev([&]() { Dev(); });

		tDataSetMainControl::tStateGNSS StateGNSSPrev = g_DataSetMainControl.Thread_GNSS_State;

		while (true)
		{
			if (g_DataSetMainControl.Thread_GNSS_State != tDataSetMainControl::tStateGNSS::Nothing)
			{
				switch (g_DataSetMainControl.Thread_GNSS_State)
				{
				case tDataSetMainControl::tStateGNSS::Start: Dev.Start(); break;
				case tDataSetMainControl::tStateGNSS::Halt: Dev.Halt(); break;
				case tDataSetMainControl::tStateGNSS::Restart: Dev.Restart(); break;
				case tDataSetMainControl::tStateGNSS::Exit: Dev.Exit(); break;
				}

				if (g_DataSetMainControl.Thread_GNSS_State == tDataSetMainControl::tStateGNSS::Exit)
					break;

				g_DataSetMainControl.Thread_GNSS_State = tDataSetMainControl::tStateGNSS::Nothing;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		
		Thread_Dev.join();

		IO.stop();

		Thread_IO.join();

		promise.set_value("EXIT");
	}
	catch (...)
	{
		promise.set_exception(std::current_exception());
	}
}

int main(int argc, char* argv[])
{
	try
	{
		boost::property_tree::ptree PTree;
		boost::property_tree::ini_parser::read_ini(std::string(argv[0]) + ".ini", PTree);

		g_ConfigINI.Main.Model = PTree.get<std::string>("Main.Model");
		g_ConfigINI.Main.Ident = PTree.get<std::string>("Main.Ident");

		g_ConfigINI.SerialPort.ID = PTree.get<std::string>("SerialPort.ID");
		g_ConfigINI.SerialPort.BR = PTree.get<utils::tUInt32>("SerialPort.BR");

		g_ConfigINI.DB.Host = PTree.get<std::string>("DB.Host");
		g_ConfigINI.DB.User = PTree.get<std::string>("DB.User");
		g_ConfigINI.DB.Passwd = PTree.get<std::string>("DB.Passwd");
		g_ConfigINI.DB.DB = PTree.get<std::string>("DB.DB");
		g_ConfigINI.DB.Port = PTree.get<unsigned int>("DB.Port");
	}
	catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";

		return 1;//ERROR
	}
	////////////////////////////////

	dev::db::Open();

	////////////////////////////////
	std::thread Thread_Shell(dev::ThreadFunShell);
	////////////////////////////////

	std::promise<std::string> Thread_GNSS_Promise;
	auto Thread_GNSS_Future = Thread_GNSS_Promise.get_future();

	std::thread Thread_GNSS(Thread_GNSS_Handler, std::ref(Thread_GNSS_Promise));//C++11

	try
	{
		//while (true)
		//{
			std::string StrValue = Thread_GNSS_Future.get();

			std::cout << StrValue << '\n';
		//}
	}
	catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	
	dev::db::Close();

	//Thread_Shell.abort();

	Thread_GNSS.join();
	Thread_Shell.join();

	return 0;
}
