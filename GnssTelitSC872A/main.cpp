#include <devGNSS.h>
//#include <devShell.h>

#include <iostream>
#include <functional>
#include <future>

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

tConfigINI g_ConfigINI;

namespace dev
{
	void ThreadFunShell();
}

//void ThreadFun_Dev(dev::tGNSS* dev)
//{
//	while (true)
//	{
//		(*dev)();
//
//		if (dev->GetStatus() == mod::tGnssTelitSC872AStatus::Halted)
//		{
//			break;
//		}
//	}
//}

void Thread_GNSS_Handler(std::promise<std::string>& promise)
{
	dev::tLog Log(dev::tLog::LogId_GNSS);

	Log.LogSettings.Field.GNSS = 1;

	boost::asio::io_context IO;

	try
	{
		dev::tGNSS Dev(&Log, IO, promise);

		std::thread Thread_IO([&]() { IO.run(); });

		while (true)
		{
			Dev();//Blocking...

			if (Dev.GetStatus() == mod::tGnssTelitSC872AStatus::Halted)
			{
				//Thread_Dev.join();

				std::this_thread::sleep_for(std::chrono::seconds(5));

				//Thread_Dev = std::thread(ThreadFun_Dev, &Dev);

				Dev.Start();
			}
		}

		Thread_IO.join();
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

		g_ConfigINI.ComPortID = PTree.get<std::string>("SerialPort.ID");
		g_ConfigINI.ComPortBR = PTree.get<unsigned int>("SerialPort.BR");
	}
	catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";

		return 1;//ERROR
	}

	std::thread Thread_Shell(dev::ThreadFunShell);
	////////////////////////////////

	std::promise<std::string> Thread_GNSS_Promise;
	auto Thread_GNSS_Future = Thread_GNSS_Promise.get_future();

	std::thread Thread_GNSS(Thread_GNSS_Handler, std::ref(Thread_GNSS_Promise));//C++11

	try
	{
		//std::thread Thread_1([&]()
		//	{
		//		IO.run();
		//	});

		//std::thread Thread_2([&]()
		//	{
		//		IO.run();
		//	});

		//IO.run();

		while (true)
		{
			std::string StrValue = Thread_GNSS_Future.get();
		}
		

		//Thread_5.join();//it's not needed if the thread is detached

		//boost::asio::serial_port Port(IO);

		//tCommunication<> SerialPort(IO, ComPortID);

		//std::thread Thread_1([&]()
		//	{
		//		IO.run();
		//	});

		//std::thread Thread_2([&]()
		//	{
		//		IO.run();
		//	});

		//IO.run();

		////ThreadConsole.join();
		//Thread_1.join();
		//Thread_2.join();
	}
	catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	


	//for (int i = 0; i < 10; ++i)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(1));

	//	std::cout << "Preved\n";
	//}

	//Thread_5.detach();
	//std::thread::id Thread_5_ID = Thread_5.get_id();
	Thread_GNSS.join();
	Thread_Shell.join();

	return 0;
}