#include <devFiniteStateMachine.h>

#include <iostream>

void ThreadFun(int tmErr_ms, int tmPause_ms)
{
	dev::tLog Log(dev::tLog::LogId_0);

	Log.LogSettings.Field.Log_0 = 1;

	dev::tDevFiniteStateMachine Dev(&Log);

	mod::tFiniteStateMachineDataSet* DataSet = Dev.GetDataSet();

	while (true)
	{
		Dev();
		//mod::tFiniteStateMachine::tCERR Cerr = Dev();

		//if (Cerr != mod::tFiniteStateMachine::tCERR::OK)
		//{
		//	std::cout << "EXIT with an ERROR...\n";
		//	break;//EXIT with an ERROR...
		//}

		if (DataSet->Changed())
		{
			std::cout << DataSet->GetDataValue1() << '\n';
		}
	}
}

int main(int argc, char* argv[])
{
	std::thread Thread_5(ThreadFun, 1000, 50);

	for (int i = 0; i < 10; ++i)
	{
		std::this_thread::sleep_for(std::chrono::seconds(10));

		std::cout << "Preved\n";
	}

	//Thread_5.detach();
	//std::thread::id Thread_5_ID = Thread_5.get_id();

	Thread_5.join();//it's not needed because the thread is detached

	return 0;
}