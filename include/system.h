#pragma once

#include <string>
#include <chrono>

// System information functions
std::wstring getusername();
std::wstring gethostname();
std::wstring getconsole();
std::chrono::milliseconds getuptime();

//int get_package_count();
