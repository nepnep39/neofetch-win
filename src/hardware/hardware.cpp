#include <windows.h>
#include <Registry.hpp>
#include <iostream>
#include <iomanip>

#include <hardware.h>

void getresolution(int& horizontal, int& vertical, int& hz)
{
	DISPLAY_DEVICE dd{};
	dd.cb = sizeof(dd);

	EnumDisplayDevices(NULL, 0, &dd, 0);
	DEVMODE dm{};
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;

	EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm);

	horizontal = dm.dmPelsWidth;
	vertical = dm.dmPelsHeight;
	hz = dm.dmDisplayFrequency;
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

int getmem(int typesel) {

	MEMORYSTATUSEX mem{};

	mem.dwLength = sizeof(mem);

	GlobalMemoryStatusEx(&mem);

	int percent = mem.dwMemoryLoad;
	int total = mem.ullTotalPhys / 1048576;
	int used = total - mem.ullAvailPhys / 1048576;

	if (typesel == 0) {
		return percent;
	}
	else if (typesel == 1) {
		return total;
	}
	else {
		return used;
	}
}

void getdisk()
{
	ULARGE_INTEGER FreeBytesAvailableToCaller;
	ULARGE_INTEGER TotalNumberOfBytes;
	ULARGE_INTEGER TotalNumberOfFreeBytes;

	double total{};
	double free{};

	BOOL check = GetDiskFreeSpaceExA("C:\\", &FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);

	if (check)
	{
		/*	printf("GB free: %llu\n", TotalNumberOfFreeBytes.QuadPart / (1024 * 1024));
			printf("GB total: %llu\n", TotalNumberOfBytes.QuadPart / (1024 * 1024));*/

		ULONGLONG totalspace = TotalNumberOfBytes.QuadPart / 1024 / 1024;
		if (totalspace <= INT_MAX) total = static_cast<double>(totalspace);

		ULONGLONG freespace = TotalNumberOfFreeBytes.QuadPart / 1024 / 1024;
		if (freespace <= INT_MAX) free = static_cast<double>(freespace);

		double totalgb = total / 1000;
		double freegb = free / 1000;

		totalgb = std::ceil(totalgb * 100.0) / 100.0;
		freegb = std::ceil(freegb * 100.0) / 100.0;


		std::wcout << LR"(C:\ )" << totalgb << L" GB (" << freegb << L" GB free)";

	}
	else
	{
		std::wcout << L"Disk info unknown";

	}

}
