#include <iostream>
#include <windows.h>

#include <display.h>
#include <colors.h>

void drawbar(double input)
{

	std::cout << "-=[ ";

	int input1 = input / 5;

	setbarc;

	int i = 0;

	while (i < input1)
	{
		std::cout << "/";
		i++;
	}

	setdflt;

	while (i < 20)
	{
		std::cout << "/";
		i++;
	}

	std::cout << " ]=-";
}

void membar()
{
	MEMORYSTATUSEX mem{};

	mem.dwLength = sizeof(mem);

	GlobalMemoryStatusEx(&mem);

	double percent = mem.dwMemoryLoad;
	int total = mem.ullTotalPhys / 1048576;
	int avail = mem.ullAvailPhys / 1048576;

	//std::wcout << percent;

	drawbar(percent);

}

void diskbar()
{
	ULARGE_INTEGER FreeBytesAvailableToCaller;
	ULARGE_INTEGER TotalNumberOfBytes;
	ULARGE_INTEGER TotalNumberOfFreeBytes;

	double total{};
	double free{};

	BOOL check = GetDiskFreeSpaceExA("C:\\", &FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);

	if (check)
	{

		ULONGLONG totalspace = TotalNumberOfBytes.QuadPart / 1024 / 1024;
		if (totalspace <= INT_MAX) total = static_cast<double>(totalspace);

		ULONGLONG freespace = TotalNumberOfFreeBytes.QuadPart / 1024 / 1024;
		if (freespace <= INT_MAX) free = static_cast<double>(freespace);

		double totalgb = total / 1000;
		double freegb = free / 1000;

		double percent = free / total;
		percent = 1.0 - percent;
		percent = percent * 100;

		//std::wcout << percent;

		drawbar(percent);

	}
	else
	{
		std::wcout << L"Unable to draw disk bar";

	}

}

void color1() {

	// std::setlocale(LC_ALL, "en_US.UTF-16");

	int i = 0;
	// char sq = static_cast<char>(254);

	while (i < 127)
	{

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
		std::wcout << " ";
		i = i + 4;

	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
}

void color2() {

	int i = 128;
	int n = 0;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);

	while (n < 4)
	{
		std::wcout << " ";
		n++;
	}

	while (i < 254)
	{

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
		std::wcout << " ";
		i = i + 4;

	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
}
