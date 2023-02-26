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
		// don't ask

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
	int c;

	HANDLE terminal = GetStdHandle(STD_OUTPUT_HANDLE);

	// win10 terminal colour should be 3, win11 colour should be 9

	if (getwinver().find(win10) != std::string::npos) {

		c = 3;

		SetConsoleTextAttribute(terminal, c);
		return artint = 0;

	}
	else if (getwinver().find(win11) != std::string::npos) {

		c = 9;

		SetConsoleTextAttribute(terminal, c);
		return artint = 1;

	}
	else {
		
		c = 3;

		SetConsoleTextAttribute(terminal, c);
		return artint = 2;

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

int debug() {

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

	if (artsel() == 0) {

		std::wcout << win10art << std::endl;
	}
	else if (artsel() == 1) {

		std::wcout << win11art << std::endl;
	}
	else if (artsel() == 2) {

		std::wcout << unknownart << std::endl;
	}
	else {	

		return 0;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	return 0;

}

void neofetch() {

	// actual render

	if (artsel() == 0) {

		std::wcout << std::left << win10art;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::wcout << std::setw(40) << std::right << getcpu() << std::endl;
	}
	else if (artsel() == 1) {

		std::wcout << std::left << win11art << std::setw(40) << std::right << "Sample text" << std::endl;
	}
	else if (artsel() == 2) {

		std::wcout << std::left << unknownart << std::setw(40) << std::right << "Sample text" << std::endl;
	}
	else {
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << "Unable to determine Windows version." << std::endl;
		std::system("PAUSE");

	}
}

int main()
{
	//debug();

	neofetch();
	return 0;
}