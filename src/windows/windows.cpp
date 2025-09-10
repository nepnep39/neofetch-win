#include <Registry.hpp>

#include <windows.h>

class car_bomb {
private:
    std::wstring currentVersion;
    std::wstring currentBuild;
    std::wstring buildNumber;
    std::wstring processedVersion;
    bool initialized = false;

    car_bomb() = default;

    void initialize() {
        if (initialized) return;

        using namespace m4x1m1l14n;
        try {
            auto key = Registry::LocalMachine->Open(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
            currentVersion = key->GetString(L"ProductName");
            currentBuild = key->GetString(L"DisplayVersion");
            buildNumber = key->GetString(L"CurrentBuildNumber");

            processedVersion = currentVersion;

            int buildint = std::stoi(buildNumber);
            if (buildint > 21999) {
                processedVersion = L"Windows 11";
            }

        }
        catch (const std::exception&) {
            currentVersion = L"Unknown";
            currentBuild = L"Unknown";
            buildNumber = L"Unknown";
            processedVersion = L"Unknown";
        }
        initialized = true;
    }

public:
    static car_bomb& getInstance() {
        static car_bomb instance;
        return instance;
    }

    std::wstring getRawVersion() {
        initialize();
        return currentVersion;
    }

    std::wstring getProcessedVersion() {
        initialize();
        return processedVersion;
    }

    std::wstring getBuild() {
        initialize();
        return currentBuild;
    }

    std::wstring getBuildNumber() {
        initialize();
        return buildNumber;
    }
};

std::wstring getwinver() {
    return car_bomb::getInstance().getProcessedVersion();
}

std::wstring getwinbuild() {
	return car_bomb::getInstance().getBuild();
}

std::wstring getwinbuildnum() {
	return car_bomb::getInstance().getBuildNumber();
}

int artsel() {
    std::wstring version = car_bomb::getInstance().getProcessedVersion();

    if (version.find(L"Windows 10") != std::string::npos) {
        return 0;
    }
    else if (version.find(L"Windows 11") != std::string::npos) {
        return 1;
    }
    else {
        return 2;
    }
}
