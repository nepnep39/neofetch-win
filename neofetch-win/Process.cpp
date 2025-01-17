#include "Process.hpp"

#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>
#include <Psapi.h>

/// <summary>
/// Gets the PID of the parent process.
/// </summary>
/// <returns>Zero if there is no parent or the Toolhelp32 snapshop could not be created. Otherwise the PID of the parent process is returned.</returns>
DWORD get_parent_process_id()
{
    DWORD current_pid = GetCurrentProcessId();
    DWORD parent_pid = 0;

    HANDLE snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot_handle == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }

    PROCESSENTRY32 process_entry = { 0 };
    process_entry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot_handle, &process_entry))
    {
        do
        {
            if (process_entry.th32ProcessID == current_pid)
            {
                parent_pid = process_entry.th32ParentProcessID;
                break;
            }
        } while (Process32Next(snapshot_handle, &process_entry));
    }

    CloseHandle(snapshot_handle);

    return parent_pid;
}

/// <summary>
/// Gets a handle to the parent process.
/// </summary>
/// <returns>INVALID_HANDLE_VALUE if get_parent_process_id or OpenProcess fails. Otherwise the HANDLE is returned.</returns>
HANDLE get_parent_process()
{
    DWORD parent_pid = get_parent_process_id();
    if (parent_pid == 0) {
        return INVALID_HANDLE_VALUE;
    }

    HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, parent_pid);
    return process_handle;
}

/// <summary>
/// Gets the process name (module base name) of a process the handle points to.
/// </summary>
/// <param name="process_handle">The handle of the process</param>
/// <returns>An empty wstring if GetModuleBaseName fails. Otherwise the name of the process is returned as a wide string.</returns>
std::wstring get_process_name(HANDLE process_handle)
{
    WCHAR process_name[MAX_PATH] = { 0 };
    if (GetModuleBaseName(process_handle, nullptr, process_name, MAX_PATH) > 0)
    {
        CloseHandle(process_handle);
        return process_name;
    }

    CloseHandle(process_handle);
    return L"";
}
