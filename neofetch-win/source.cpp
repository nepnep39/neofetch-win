#include <iostream>
#include <windows.h>
#include <string.h>
#include <Lmcons.h>
#include <chrono>

#include <art.h>
#include <Registry.hpp>

#define setlght SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3)
#define setdflt SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7)

// win10 ascii art terminal colour should be 3, default (for text) is 7
// you can modify these defines to customize the output colours!

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

	try {
		auto key = Registry::LocalMachine->Open(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");

		auto car_bomb = key->GetString(L"ProductName");
		// don't ask

		/*std::wstring ws = key->GetString(L"CurrentBuild");
		int buildint = std::stoi(ws);

		if (buildint > 21999) {
			
			car_bomb = L"Windows 11 " + key->GetString(L"EditionID");
			return car_bomb;
		}
		else {
			car_bomb = L"Windows 10 " + key->GetString(L"EditionID");
			return car_bomb;
		}*/

		return car_bomb;
	}
	catch (const std::exception&)
	{
		car_bomb = L"Unknown";
		return car_bomb;
	}

}

//std::wstring getwinver2() {
//
//	using namespace m4x1m1l14n;
//
//	try {
//
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

	try {
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
		std::wstring error = L"Unknown (try running WinSAT to fix this)";
		return error;
	}

}
int artsel() {

	std::wstring win10 = L"Windows 10";
	std::wstring win11 = L"Windows 11";

	int artint;

	if (getwinver().find(win10) != std::string::npos) {

		return artint = 0;

	}
	else if (getwinver().find(win11) != std::string::npos) {

		return artint = 1;

	}
	else {
		
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

void colourbar() {

	int i = 0;
	char sq = static_cast<char>(254);
	while (i < 16)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
		std::wcout << sq << sq << sq;
		i++;

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

	return 0;

}

void neofetch() {

	// actual render

	std::wstring wide0(getusername());

	std::string username;
	std::transform(wide0.begin(), wide0.end(), std::back_inserter(username), [](wchar_t c) {
		return (char)c;
		});

	//username = "Administrator";

	std::wstring wide1(gethostname());

	std::string hostname;
	std::transform(wide1.begin(), wide1.end(), std::back_inserter(hostname), [](wchar_t c) {
		return (char)c;
		});

	/*std::cout << username << std::endl;
	std::cout << hostname << std::endl;

	std::cout << username.length() << std::endl;
	std::cout << hostname.length() << std::endl;*/

	std::wstring divider = L"--------------";

	int uptime = std::chrono::duration_cast<std::chrono::milliseconds>(getuptime()).count();

	int minutes = (int)((uptime / (1000 * 60)) % 60);
	int hours = (int)((uptime / (1000 * 60 * 60)) % 24);

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

	int horizontal = 0;
	int vertical = 0;
	getresolution(horizontal, vertical);

	if (artsel() == 0) {

		using namespace std;

		setlght; wcout << win10art01 << setw(19) << endl;
		setlght; wcout << win10art02 << setw(12 + username.length()) << right << getusername(); setdflt; cout << '@'; setlght;  wcout << gethostname() << endl; setdflt;
		setlght; wcout << win10art03; setdflt; wcout << setw(26) << right << divider << endl;
		setlght; wcout << win10art04 << setw(16) << right << L"OS: "; setdflt; wcout << getwinver() << endl;
		setlght; wcout << win10art05 << setw(19) << right << L"Build: "; setdflt; wcout << getwinbuild() << endl;
		setlght; wcout << win10art06 << setw(20) << right << L"Uptime: "; setdflt; cout << hours << " " << plural1 << ", " << minutes << " " << plural2 << std::endl;
		setlght; wcout << win10art07 << setw(24) << right << L"Resolution: "; setdflt; cout << horizontal << 'x' << vertical << std::endl;
		setlght; wcout << win10art08 << setw(22) << right << L"Terminal: "; setdflt; wcout << getconsole() << endl;
		setlght; wcout << win10art09 << setw(17) << right << L"CPU: "; setdflt; wcout << getcpu() << endl;
		setlght; wcout << win10art10 << setw(17) << right << L"GPU: "; setdflt; wcout << getgpu() << endl;
		setlght; wcout << win10art11 << setw(20) << right << L"Memory: "; setdflt; cout << getmem(2) << " MB / " << getmem(1) << " MB (" << getmem(0) << "% in use)" << endl;
		setlght; wcout << win10art12 << setw(19) << endl;
		setlght; wcout << win10art09 << setw(13) << right;  colourbar(); cout << endl;
		setlght; wcout << win10art14 << setw(19) << endl;
		setlght; wcout << win10art15 << setw(19) << endl;
		setlght; wcout << win10art16 << setw(19) << endl;
		setlght; wcout << win10art17 << setw(19) << endl;
		setlght; wcout << win10art18 << setw(19) << endl;
		setlght; wcout << win10art19 << setw(19) << endl;
		setlght; wcout << win10art20 << setw(19) << endl;

		setdflt;

	}
	else if (artsel() == 1) {

		using namespace std;

		setlght; wcout << win11art01 << setw(12 + username.length()) << right << getusername(); setdflt; cout << '@'; setlght;  wcout << gethostname() << endl; setdflt;
		setlght; wcout << win11art02; setdflt; wcout << setw(26) << right << divider << endl;
		setlght; wcout << win11art03 << setw(16) << right << L"OS: "; setdflt; wcout << getwinver() << endl;
		setlght; wcout << win11art04 << setw(19) << right << L"Build: "; setdflt; wcout << getwinbuild() << endl;
		setlght; wcout << win11art05 << setw(20) << right << L"Uptime: "; setdflt; cout << hours << " " << plural1 << ", " << minutes << " " << plural2 << std::endl;
		setlght; wcout << win11art06 << setw(24) << right << L"Resolution: "; setdflt; cout << horizontal << 'x' << vertical << std::endl;
		setlght; wcout << win11art07 << setw(22) << right << L"Terminal: "; setdflt; wcout << getconsole() << endl;
		setlght; wcout << win11art08 << setw(17) << right << L"CPU: "; setdflt; wcout << getcpu() << endl;
		setlght; wcout << win11art09 << setw(17) << right << L"GPU: "; setdflt; wcout << getgpu() << endl;
		setlght; wcout << win11art10 << setw(20) << right << L"Memory: "; setdflt; cout << getmem(2) << " MB / " << getmem(1) << " MB (" << getmem(0) << "% in use)" << endl;
		setlght; wcout << win11art11 << setw(19) << endl;
		setlght; wcout << win11art12 << setw(13) << right;  colourbar(); cout << endl;
		setlght; wcout << win11art13 << setw(19) << endl;
		setlght; wcout << win11art14 << setw(19) << endl;
		setlght; wcout << win11art15 << setw(19) << endl;
		setlght; wcout << win11art16 << setw(19) << endl;
		setlght; wcout << win11art17 << setw(19) << endl;
		setlght; wcout << win11art18 << setw(19) << endl;
		setlght; wcout << win11art19 << setw(19) << endl;

		setdflt;
	
	}
	else if (artsel() == 2) {

		using namespace std;

		setlght; wcout << unknownart01 << setw(12 + username.length()) << right << getusername(); setdflt; cout << '@'; setlght;  wcout << gethostname() << endl; setdflt;
		setlght; wcout << unknownart02; setdflt; wcout << setw(26) << right << divider << endl;
		setlght; wcout << unknownart03 << setw(16) << right << L"OS: "; setdflt; wcout << getwinver() << endl;
		setlght; wcout << unknownart04 << setw(19) << right << L"Build: "; setdflt; wcout << getwinbuild() << endl;
		setlght; wcout << unknownart05 << setw(20) << right << L"Uptime: "; setdflt; cout << hours << " " << plural1 << ", " << minutes << " " << plural2 << std::endl;
		setlght; wcout << unknownart06 << setw(24) << right << L"Resolution: "; setdflt; cout << horizontal << 'x' << vertical << std::endl;
		setlght; wcout << unknownart07 << setw(22) << right << L"Terminal: "; setdflt; wcout << getconsole() << endl;
		setlght; wcout << unknownart08 << setw(17) << right << L"CPU: "; setdflt; wcout << getcpu() << endl;
		setlght; wcout << unknownart09 << setw(17) << right << L"GPU: "; setdflt; wcout << getgpu() << endl;
		setlght; wcout << unknownart10 << setw(20) << right << L"Memory: "; setdflt; cout << getmem(2) << " MB / " << getmem(1) << " MB (" << getmem(0) << "% in use)" << endl;
		setlght; wcout << unknownart11 << setw(19) << endl;
		setlght; wcout << unknownart12 << setw(13) << right;  colourbar(); cout << endl;
		setlght; wcout << unknownart13 << setw(19) << endl;
		setlght; wcout << unknownart14 << setw(19) << endl;
		setlght; wcout << unknownart15 << setw(19) << endl;
		setlght; wcout << unknownart16 << setw(19) << endl;
		setlght; wcout << unknownart17 << setw(19) << endl;
		setlght; wcout << unknownart18 << setw(19) << endl;
		setlght; wcout << unknownart19 << setw(19) << endl;

		setdflt;
		
	}
	else {
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << "Unable to determine Windows version." << std::endl;
		system("PAUSE");

	}
}

int main()
{
	//debug();

	neofetch();
	return 0;
}