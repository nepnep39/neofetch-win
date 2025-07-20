#pragma once

#include <string>
#include <vector>

std::string wstring_to_string(const std::wstring& wide);

std::wstring get_art_config_path();
std::vector<std::wstring> load_custom_art();
