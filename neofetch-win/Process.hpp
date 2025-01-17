#pragma once

#include <string>
#include <Windows.h>

DWORD get_parent_process_id();
HANDLE get_parent_process();
std::wstring get_process_name(HANDLE process_handle);
