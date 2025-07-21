#include <windows.h>
#include <Lmcons.h>
#include <chrono>

#include <system.h>

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

std::wstring getconsole() {

    WCHAR console[UNLEN + 1]{};
    GetConsoleTitleW((WCHAR*)console, 256);

    std::wstring consolestring = console;
    return console;
}

std::chrono::milliseconds getuptime() {
    auto uptime = std::chrono::milliseconds(GetTickCount64());
    return uptime;
}


// Wont use that, powershell sucks ass
/*
int get_package_count() {
    HANDLE hPipeRead, hPipeWrite;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    if (!CreatePipe(&hPipeRead, &hPipeWrite, &sa, 0)) {
        return 0;
    }

    SetHandleInformation(hPipeRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = hPipeWrite;
    si.hStdError = hPipeWrite;

    PROCESS_INFORMATION pi = { 0 };

    char cmd[] = "powershell.exe -Command \"@(winget list --source winget).Count\"";
    if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hPipeWrite);
        CloseHandle(hPipeRead);
        return 0;
    }

    CloseHandle(hPipeWrite);

    std::string result;
    char buffer[64];
    DWORD bytesRead;

    while (ReadFile(hPipeRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        result += buffer;
    }

    WaitForSingleObject(pi.hProcess, 100);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hPipeRead);

    if (!result.empty()) {

        result.erase(result.find_last_not_of(" \t\r\n") + 1);
        try {
            return std::stoi(result);
        }
        catch (...) {
            return 0;
        }
    }

    return 0;
}
*/
