#include <windows.h>
#include <dxgi.h>
#include <format>
#include <Registry.hpp>

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
	std::wstring gpuDesc;
	size_t vramMB = 0;

	// Find the primary display device
	DISPLAY_DEVICEW dd{};
	dd.cb = sizeof(dd);
	DWORD devNum = 0;
	std::wstring primaryDeviceID;
	while (EnumDisplayDevicesW(nullptr, devNum, &dd, 0)) {
		if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
			primaryDeviceID = dd.DeviceID;
			gpuDesc = dd.DeviceString;
			break;
		}
		devNum++;
	}

	// use DXGI to get gpu details (VRAM size), Lazy load dxgi.dll to avoid dependency issues on older systems
	HMODULE hDXGI = LoadLibraryW(L"dxgi.dll");
	if (hDXGI) {
		typedef HRESULT(WINAPI* PFN_CreateDXGIFactory)(REFIID, void**);
		PFN_CreateDXGIFactory pCreateDXGIFactory = (PFN_CreateDXGIFactory)GetProcAddress(hDXGI, "CreateDXGIFactory");
		if (pCreateDXGIFactory) {
			IDXGIFactory* pFactory = nullptr;
			HRESULT hr = pCreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
			if (SUCCEEDED(hr) && pFactory) {
				
				// Match DXGI adapter to primary display device
				IDXGIAdapter* pAdapter = nullptr;
				for (UINT i = 0; SUCCEEDED(pFactory->EnumAdapters(i, &pAdapter)); ++i) {
					DXGI_ADAPTER_DESC desc;
					if (SUCCEEDED(pAdapter->GetDesc(&desc))) {
						// Convert LUID to string to match with DeviceID
						WCHAR luidStr[64];
						swprintf_s(luidStr, L"PCI\\VEN_%04X&DEV_%04X", desc.VendorId, desc.DeviceId);
						if (!primaryDeviceID.empty() && wcsstr(primaryDeviceID.c_str(), luidStr)) {
							gpuDesc = desc.Description;
							SIZE_T vramBytes = desc.DedicatedVideoMemory;
							vramMB = static_cast<size_t>(vramBytes / (1024 * 1024));
							pAdapter->Release();
							break;
						}
					}
					pAdapter->Release();
				}
				pFactory->Release();
			}
		}
		FreeLibrary(hDXGI);
	}

	if (!gpuDesc.empty()) {
		if(vramMB == 0)
			return gpuDesc;
		else
			return std::format(L"{} ({} MB)", gpuDesc, vramMB);
	}

	// fallack to registry method if above fails
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
		ULONGLONG totalspace = TotalNumberOfBytes.QuadPart / 1024 / 1024;
		if (totalspace <= INT_MAX) total = static_cast<double>(totalspace);

		ULONGLONG freespace = TotalNumberOfFreeBytes.QuadPart / 1024 / 1024;
		if (freespace <= INT_MAX) free = static_cast<double>(freespace);

		double totalgb = total / 1000;
		double freegb = free / 1000;

		totalgb = (totalgb * 100.0) / 100.0;
		freegb = (freegb * 100.0) / 100.0;

		printf("C:\\ %.2f GB (%.2f GB free)", totalgb, freegb);
	}
	else
	{
		printf("Disk info unknown");
	}
}
