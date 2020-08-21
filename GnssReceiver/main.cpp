#include <devConfig.h>

#include <devDB.h>
#include <devGNSS.h>
#include <devSettings.h>
//#include <devShell.h>

#include <atomic>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>

#include <boost/asio.hpp>

namespace dev
{
	void ThreadFunShell();
}

tDataSetMainControl g_DataSetMainControl;

void Thread_GNSS_Handler(std::promise<std::string>& promise)
{
	dev::tLog Log(dev::tLog::LogId_GNSS);

	Log.LogSettings.Field.GNSS = 1;

	boost::asio::io_context IO;

	try
	{
		dev::tGNSS Dev(&Log, IO);

		std::thread Thread_IO([&]() { IO.run(); });

		bool Thread_Dev_Exception = false;
		std::thread Thread_Dev([&]()
			{
				try
				{
					Dev();
				}
				catch (...)
				{
					Thread_Dev_Exception = true;

					promise.set_exception(std::current_exception());
				}
			});

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
				case tDataSetMainControl::tStateGNSS::UserTaskScriptStart:
				{
					std::lock_guard<std::mutex> Lock(g_DataSetMainControl.Thread_GNSS_State_UserTaskScriptIDMtx);

					if (!g_DataSetMainControl.Thread_GNSS_State_UserTaskScriptID.empty())
					{
						Dev.StartUserTaskScript(g_DataSetMainControl.Thread_GNSS_State_UserTaskScriptID);

						g_DataSetMainControl.Thread_GNSS_State_UserTaskScriptID.clear();
					}
					break;
				}
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

		if (!Thread_Dev_Exception)
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
		dev::g_Settings = dev::tSettings(std::string(argv[0]) + ".cfg");
	}
	catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";

		return 1;//ERROR
	}
	////////////////////////////////

	try
	{
		dev::db::Open();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";

		return 1;//ERROR
	}

	////////////////////////////////
	std::thread Thread_Shell(dev::ThreadFunShell);
	////////////////////////////////

	std::promise<std::string> Thread_GNSS_Promise;
	auto Thread_GNSS_Future = Thread_GNSS_Promise.get_future();

	std::thread Thread_GNSS(Thread_GNSS_Handler, std::ref(Thread_GNSS_Promise));//C++11

	bool Exit = false;

	try
	{
		//while (true)
		//{
			std::string StrValue = Thread_GNSS_Future.get();

			std::cout << StrValue << '\n';
		//}

		Exit = true;
	}
	catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";

		g_DataSetMainControl.Thread_GNSS_State = tDataSetMainControl::tStateGNSS::Exit;
	}

	dev::db::Close();

	Thread_GNSS.join();

	if (!Exit)
		std::cout << "Press ENTER...";

	Thread_Shell.join();

	return 0;
}
