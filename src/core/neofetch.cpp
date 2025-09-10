#include <iostream>
#include <windows.h>
#include <chrono>
#include <iomanip>
#include <vector>

#include <art.h>
#include <Registry.hpp>
#include <system.h> 
#include <hardware.h>
#include <windowsinfo.h>
#include <display.h>
#include <colors.h>
#include "utils.h" 
#include <debug.h>

void render_neofetch_display(int art_type) {

    std::string username = wstring_to_string(getusername());
    std::string hostname = wstring_to_string(gethostname());

    int uptime = std::chrono::duration_cast<std::chrono::seconds>(getuptime()).count();
    int days = uptime / (60 * 60 * 24);
    int hours = (uptime / (60 * 60)) % 24;
    int minutes = (uptime / 60) % 60;

    int horizontal = 0, vertical = 0, hz = 0;
    getresolution(horizontal, vertical, hz);

    using namespace std;

    //int package_count = get_package_count();

    const char* plural1 = (hours == 1) ? "hour" : "hours";
    const char* plural2 = (minutes == 1) ? "minute" : "minutes";
    const char* plural3 = (days == 1) ? "day" : "days";

    std::wstring divider = L"--------------";

    std::vector<std::wstring> custom_art = load_custom_art();

    std::vector<std::wstring> art_lines;

    if (!custom_art.empty()) {
        // use custom art if available
        art_lines = custom_art;
    }
    else if (art_type == 0) {
        // Windows 10
        art_lines = {
            win10art01, win10art02, win10art03, win10art04, win10art05,
            win10art06, win10art07, win10art08, win10art09, win10art10,
            win10art11, win10art12, win10art13, win10art14, win10art15,
            win10art16, win10art17, win10art18, win10art19, win10art20
        };
    }
    else {
        // Windows 11
        art_lines = {
            win11art01, win11art02, win11art03, win11art04, win11art05,
            win11art06, win11art07, win11art08, win11art09, win11art10,
            win11art11, win11art12, win11art13, win11art14, win11art15,
            win11art16, win11art17, win11art18, win11art19
        };
    }

    using namespace std;

    while (art_lines.size() < 19) {
        art_lines.push_back(L"");
    }

    setlght; wcout << art_lines[0] << setw(19) << endl;
    setlght; wcout << art_lines[1] << setw(12 + username.length()) << right << getusername(); setdflt; cout << '@'; setlght; wcout << gethostname() << endl; setdflt;
    setlght; wcout << art_lines[2]; setdflt; wcout << setw(26) << right << divider << endl;
    setlght; wcout << art_lines[3] << setw(16) << right << L"OS: "; setdflt; wcout << getwinver() << endl;
    setlght; wcout << art_lines[4] << setw(19) << right << L"Build: "; setdflt; wcout << getwinbuild() << " (" << getwinbuildnum() << ')' << endl;
    setlght; wcout << art_lines[5] << setw(20) << right << L"Uptime: "; setdflt; cout << days << " " << plural3 << ", " << hours << " " << plural1 << ", " << minutes << " " << plural2 << endl;
    setlght; wcout << art_lines[6] << setw(24) << right << L"Resolution: "; setdflt; cout << horizontal << 'x' << vertical << " @" << hz << "Hz" << endl;
    setlght; wcout << art_lines[7] << setw(22) << right << L"Terminal: "; setdflt; wcout << getconsole() << endl;
    setlght; wcout << art_lines[8] << setw(17) << right << L"CPU: "; setdflt; wcout << getcpu() << endl;
    setlght; wcout << art_lines[9] << setw(17) << right << L"GPU: "; setdflt; wcout << getgpu() << endl;
    setlght; wcout << art_lines[10] << setw(20) << right << L"Memory: "; setdflt; cout << getmem(2) << " MB / " << getmem(1) << " MB (" << getmem(0) << "% in use)" << endl;
    //setlght; wcout << art_lines[11] << setw(22) << right << L"Packages: "; setdflt; cout << package_count << " (winget)" << endl;
    setlght; wcout << art_lines[11] << setw(18) << right << L"Disk: "; setdflt; getdisk(); cout << endl;
    setlght; wcout << art_lines[12] << setw(19) << endl;
    setlght; wcout << art_lines[13] << setw(19) << right << L"Mem%:  "; setdflt; membar(); cout << endl;
    setlght; wcout << art_lines[14] << setw(19) << endl;
    setlght; wcout << art_lines[15] << setw(19) << right << L"Disk%: "; setdflt; diskbar(); cout << endl;
    setlght; wcout << art_lines[16] << setw(19) << endl;
    setlght; wcout << art_lines[17] << setw(13) << right; color1(); cout << endl;
    setlght; wcout << art_lines[18] << setw(9) << right; color2(); cout << endl;

    for (size_t i = 19; i < art_lines.size(); i++) {
        setlght; wcout << art_lines[i] << endl;
    }

    setdflt;
}

void neofetch() {
    render_neofetch_display(artsel());
}
