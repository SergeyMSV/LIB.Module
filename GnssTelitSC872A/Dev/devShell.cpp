#include "devShell.h"

#include <iostream>

namespace dev
{

tShell* g_pShell = nullptr;

tShell::tShell(const utils::shell::tShellCommandList* cmdList, std::size_t cmdListSize)
	:utils::shell::tShell(cmdList, cmdListSize)
{
	g_pShell = this;

	OnShell();//
}

tShell::~tShell()
{
	g_pShell = nullptr;
}

bool tShell::Handler1(const std::vector<std::string>& data)
{
	std::cout << "Handler1" << std::endl;

	ShowReceivedCmd(data);

	return true;
}

bool tShell::Handler2(const std::vector<std::string>& data)
{
	std::cout << "Handler2" << std::endl;

	ShowReceivedCmd(data);

	if (data.size() == 1)
	{
		const char MenuFormat[] = " %-20s %s\n";

		for (unsigned int i = 0; i < g_ShellCommandList.size(); ++i)
		{
			if (g_ShellCommandList[i].Command == 0)
			{
				break;
			}

			if (g_ShellCommandList[i].Command[0] == '~')//Hidden line
			{
				////////////////////////////////
				//Hidden menu
				std::cout << " hidden line ---- !!!" << std::endl;
				////////////////////////////////
				////////////////////////////////
			}
			else
			{
				std::printf(MenuFormat, g_ShellCommandList[i].Command, g_ShellCommandList[i].Description);
			}
		}

		return true;
	}

	return false;
}

bool tShell::Handler3(const std::vector<std::string>& data)
{
	std::cout << "Handler3" << std::endl;

	ShowReceivedCmd(data);

	Debug = !Debug;

	return true;
}

bool tShell::Handler4(const std::vector<std::string>& data)
{
	std::cout << "Handler4" << std::endl;

	if (data.size() > 1 && g_pShell)
	{
		g_pShell->SetEcho(data[1] != "0");

		return true;
	}

	return false;
}

void tShell::Board_Send(char data) const
{
	std::cout << data;
}

void tShell::Board_Send(const std::vector<char>& data) const
{
	for (size_t i = 0; i < data.size(); ++i)
	{
		std::cout << data[i];
	}
}

void tShell::OnShell() const
{
	std::cout << "#";
	//if (Debug)
	//{
	//	std::cout << "debug>>";
	//}
	//else
	//{
	//	std::cout << "shell>>";
	//}
}

void tShell::OnFailed(std::vector<std::string>& data) const
{
	std::cout << "OnFailed:" << std::endl;

	ShowReceivedCmd(data);
}

void tShell::ShowReceivedCmd(const std::vector<std::string>& data)
{
	for (size_t i = 0; i < data.size(); ++i)
	{
		std::cout << i << " " << data[i] << std::endl;
	}
}

bool tShell::Debug = false;

void ThreadFunShell()
{
	static const std::vector<utils::shell::tShellCommandList> ShellCommandList
	{
		{ (char*)"help",   (char*)"blablabla", tShell::Handler1 },
		{ (char*)"?",      (char*)"la-la-la",  tShell::Handler2 },
		{ (char*)"~2",     (char*)"bla-bla",   tShell::Handler3 },
		{ (char*)"~debug", (char*)"DEBUG",     tShell::Handler3 },
		{ (char*)"echo",   (char*)"ECHO 0-off, 1-on",      tShell::Handler4 },
		{ 0 }
	};

	dev::tShell Shell(ShellCommandList.data(), ShellCommandList.size());

	while (true)
	{
		int Byte = getchar();

		Shell.Board_OnReceived(static_cast<char>(Byte));
	}
}

}
