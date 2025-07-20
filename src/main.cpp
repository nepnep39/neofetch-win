#include <iostream>
#include <windows.h>
#include <locale>

#include <debug.h>

void neofetch();
int debug();

int main(int argc, char* argv[]) {

    SetConsoleOutputCP(CP_UTF8);
    std::setlocale(LC_ALL, "en_US.UTF-8");


    bool debugMode = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-d" || arg == "--debug") {
            debugMode = true;
        }
        if (arg == "-h" || arg == "--help") {
            fprintf(stdout, "Usage: neofetch-win [-d|--debug] [-h|--help]\n");
            fprintf(stdout, "Github: https://github.com/nepnep39/neofetch-win\n");
			exit(0);
        }
    }

    if (debugMode) {
        debug();
    }
    else {
        neofetch();
    }

    return 0;
}
