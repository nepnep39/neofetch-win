#include <iostream>
#include <windows.h>
#include <string.h>
#include <Lmcons.h>
#include <chrono>

#include <art.h>
#include <Registry.hpp>

std::wstring getusername() {
	
	TCHAR username[UNLEN + 1]{};
	DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)username, &size);

	std::wstring userstring = username;

	return userstring;
}

std::wstring gethostname() {

	TCHAR hostname[UNLEN + 1]{};
	DWORD size = UNLEN + 1;
	GetComputerName((TCHAR*)hostname, &size);

	std::wstring userhost = hostname;

	return userhost;
}

std::wstring getwinver() {

	std::wstring car_bomb;
	using namespace m4x1m1l14n;

	try

	{
		auto key = Registry::LocalMachine->Open(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");

		auto car_bomb = key->GetString(L"ProductName");

		return car_bomb;
	}
	catch (const std::exception&)
	{
		std::wstring error = L"Unknown";
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

		auto build = key->GetString(L"DisplayVersion");

		return build;
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

	TCHAR console[UNLEN + 1]{};
	GetConsoleTitle((TCHAR*)console, 256);

	std::wstring consolestring = console;

	return consolestring;
}

std::chrono::milliseconds getuptime() {

	auto uptime = std::chrono::milliseconds(GetTickCount64());
	return uptime;

}

std::wstring getcpu() {

	using namespace m4x1m1l14n;

	try
	{
		auto key = Registry::LocalMachine->Open(L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");

		auto cpuname = key->GetString(L"ProcessorNameString");

		return cpuname;
	}
	catch (const std::exception&)
	{
		std::wstring error = L"Unknown processor";
		return error;
	}

}

std::wstring getgpu() {

	using namespace m4x1m1l14n;

	try
	{
		auto key = Registry::LocalMachine->Open(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\WinSAT");

		auto gpuname = key->GetString(L"PrimaryAdapterString");

		return gpuname;
	}
	catch (const std::exception&)
	{
		std::wstring error = L"Unknown";
		return error;
	}

}
int artsel() {

	std::wstring win10 = L"Windows 10";
	std::wstring win11 = L"Windows 11";
	int artint;

	if (getwinver().find(win10) != std::string::npos) {
		artint = 0;
		return artint;
	}
	else if (getwinver().find(win11) != std::string::npos) {
		artint = 1;
		return artint;
	}
	else {
		artint = 2;
		return artint;
	}

}

int getmem(int typesel) {

	MEMORYSTATUSEX mem{};

	mem.dwLength = sizeof(mem);

	GlobalMemoryStatusEx(&mem);

	int percent = mem.dwMemoryLoad;
	int total = mem.ullTotalPhys / 1048576;
	int avail = mem.ullAvailPhys / 1048576;

	if (typesel == 0) {
		return percent;
	}
	else if (typesel == 1) {
		return total;
	}
	else {
		return avail;
	}
}

void debug() {

	// calling this function lists all vars in a list without labels or any fancy stuff

	std::wcout << getusername() << std::endl;
	std::wcout << gethostname() << std::endl;
	std::wcout << getwinver() << std::endl;
	//std::wcout << getwinver2() << std::endl;
	std::wcout << getwinbuild() << std::endl;

	int uptime = std::chrono::duration_cast<std::chrono::milliseconds>(getuptime()).count();

	/*int seconds = (int)(uptime / 1000) % 60;*/
	int minutes = (int)((uptime / (1000 * 60)) % 60);
	int hours = (int)((uptime / (1000 * 60 * 60)) % 24);

	//nevermind who needs seconds anyways

	/*while (seconds < 10)
	{
		std::string seconds = seconds;
		seconds = '0' + seconds;
		break;

	}*/

	std::string plural1;
	std::string plural2;

	if (hours == 1)
	{
		plural1 = "hour";
	}
	else {
		plural1 = "hours";
	}

	if (minutes == 1)
	{
		plural2 = "minute";
	}
	else {
		plural2 = "minutes";
	}

	std::cout << hours <<" " << plural1 << ", " << minutes << " " << plural2 << std::endl;
	
	int horizontal = 0;
	int vertical = 0;
	getresolution(horizontal, vertical);
	std::cout << horizontal << 'x' << vertical << std::endl;

	std::wcout << getconsole() << std::endl;
	std::wcout << getcpu() << std::endl;
	std::wcout << getgpu() << std::endl;

	std::cout << getmem(2) << " MB / " << getmem(1) << " MB (" << getmem(0) << "% in use)";

	// win10 colour should be 3, win11 colour should be 9

	HANDLE terminal;
	terminal = GetStdHandle(STD_OUTPUT_HANDLE);
	int c;

	if (artsel() == 0) {
		int c = 3;
		SetConsoleTextAttribute(terminal, c);
		std::cout << win10art << std::endl;
	}
	else if (artsel() == 1) {
		int c = 9;
		SetConsoleTextAttribute(terminal, c);
		std::cout << win11art << std::endl;
	}
	else {
		int c = 3;
		SetConsoleTextAttribute(terminal, c);
		std::cout << unknownart << std::endl;
	}

	SetConsoleTextAttribute(terminal, 7);

}

int main()
{
	debug();
	return 0;
}
