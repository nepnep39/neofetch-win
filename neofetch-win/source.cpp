#include <iostream>
#include <windows.h>
#include <string.h>
#include <Lmcons.h>
#include <Registry.hpp>

std::wstring getusername() {
	
	TCHAR username[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)username, &size);

	std::wstring userstring = username;

	return userstring;
}

std::wstring gethostname() {

	TCHAR hostname[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetComputerName((TCHAR*)hostname, &size);

	std::wstring userhost = hostname;

	return userhost;
}

std::wstring getwinver() {

	using namespace m4x1m1l14n;

	try
	{
		auto key = Registry::LocalMachine->Open(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");

		auto logFileName = key->GetString(L"EditionId");

		return logFileName;
	}
	catch (const std::exception&)
	{
		
		return NULL; //change this you fucking retard
	}

	return 0;
}

std::wstring getwinbuild() {
	using namespace m4x1m1l14n;

	try
	{
		auto key = Registry::LocalMachine->Open(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");

		auto logFileName = key->GetString(L"DisplayVersion");

		return logFileName;
	}
	catch (const std::exception&)
	{

		return NULL; //again
	}

	return 0;
}

int main()
{
	//debug
	std::wcout << getusername() << std::endl;
	std::wcout << gethostname() << std::endl;
	std::wcout << getwinver() << std::endl;
	std::wcout << getwinbuild() << std::endl;

}