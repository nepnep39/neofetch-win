#include <iostream>
#include <windows.h>
#include <string.h>
#include <Lmcons.h>

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

int main()
{
	//debug
	std::wcout << getusername() << std::endl;
	std::wcout << gethostname() << std::endl;
}