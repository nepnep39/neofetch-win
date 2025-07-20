#pragma once

#include <string>

// Hardware detection functions
void getresolution(int& horizontal, int& vertical, int& hz);
std::wstring getcpu();
std::wstring getgpu();
int getmem(int typesel);
void getdisk();
