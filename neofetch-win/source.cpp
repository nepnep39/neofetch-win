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

std::wstring getwinver1() {

	std::wstring car_bomb;
	using namespace m4x1m1l14n;

	try

	{
		auto key = Registry::LocalMachine->Open(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");

		auto car_bomb = key->GetString(L"ProductName");


		// this code works but for some reason whenever i query EditionId it gives me "Enterprise" when im on windows 10 pro so we're just scrapping this i guess

		/*std::wstring s2 = L"Windows 10";
		std::wstring s3 = L"Windows 11";

		if (car_bomb.find(s2) != std::string::npos) {
			car_bomb = s2;
			return car_bomb;
		}
		else if (car_bomb.find(s3) != std::string::npos) {
			car_bomb = s3;
			return car_bomb;
		}
		else {
			car_bomb = L"Unknown Windows Version";
			return car_bomb;
		}*/

		return car_bomb;
	}
	catch (const std::exception&)
	{
		std::wstring error = L"Unknown Windows Version";
		return error;
	}

}

//std::wstring getwinver2() {
//
//	using namespace m4x1m1l14n;
//
//	try
//	{
//		auto key = Registry::LocalMachine->Open(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
//
//		auto car_bomb = key->GetString(L"EditionId");
//
//		return car_bomb;
//	}
//	catch (const std::exception&)
//	{
//		std::wstring error = L"Unknown";
//		return error;
//	}
//
//}

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
		std::wstring error = L"Unknown";
		return error;
	}

	
}

void getresolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);
	
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

std::wstring getconsole() {

	TCHAR console[UNLEN + 1];
	GetConsoleTitle((TCHAR*)console, 256);

	std::wstring consolestring = console;

	return consolestring;
}

void debug() {

	std::wcout << getusername() << std::endl;
	std::wcout << gethostname() << std::endl;
	std::wcout << getwinver1() << std::endl;
	//std::wcout << getwinver2() << std::endl;
	std::wcout << getwinbuild() << std::endl;
	
	int horizontal = 0;
	int vertical = 0;
	getresolution(horizontal, vertical);
	std::cout << horizontal << 'x' << vertical << std::endl;

	std::wcout << getconsole() << std::endl;
}

int main()
{
	debug();
	return 0;
}