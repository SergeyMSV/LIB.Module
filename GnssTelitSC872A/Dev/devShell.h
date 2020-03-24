///////////////////////////////////////////////////////////////////////////////////////////////////
// devShell.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 03 04  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>
#include <devLog.h>

#include "utilsShell.h"

namespace dev
{

class tShell : public utils::shell::tShell
{
	tShell() = delete;

public:
	explicit tShell(const utils::shell::tShellCommandList* cmdList, std::size_t cmdListSize);
	virtual ~tShell();

	static bool Handler1(const std::vector<std::string>& data);
	static bool Handler2(const std::vector<std::string>& data);

	static bool Debug;

	static bool Handler3(const std::vector<std::string>& data);
	static bool HandlerECHO(const std::vector<std::string>& data);
	static bool HandlerLog(const std::vector<std::string>& data);

protected:
	virtual void Board_Send(char data) const;
	virtual void Board_Send(const std::vector<char>& data) const;
	virtual void OnShell() const;
	virtual void OnFailed(std::vector<std::string>& data) const;

private:
	static void ShowReceivedCmd(const std::vector<std::string>& data);
};

static const std::vector<utils::shell::tShellCommandList> g_ShellCommandList
{
	{ (char*)"help",   (char*)"blablabla", tShell::Handler1 },
	{ (char*)"?",      (char*)"la-la-la",  tShell::Handler2 },
	{ (char*)"~2",     (char*)"bla-bla",   tShell::Handler3 },
	{ (char*)"~debug", (char*)"DEBUG",     tShell::Handler3 },
	{ (char*)"echo",   (char*)"ECHO 0-off, 1-on",      tShell::HandlerECHO },
	{ (char*)"log",   (char*)"log 0-off, 1-on",      tShell::HandlerLog },
	{ 0 }
};

}
