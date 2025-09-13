#include "utils.h"
#include <windows.h>
#include <shlobj.h>
#include <fstream>

// string conversion
std::string wstring_to_string(const std::wstring& wide) {
    if (wide.empty()) return "";
    int size = WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size - 1, 0);
    WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, &result[0], size, nullptr, nullptr);
    return result;
}

// configuration
// path: %USERPROFILE%\.neofetch-win or C:\Program Files\neofetch-win (prioritized)
std::wstring get_art_config_path() {
    wchar_t* the_sport_n_field_collection_will_get_removed_at_the_end_of_the_armory_pass_its_a_good_invest_trust = nullptr;
    size_t len = 0;

    if (_wdupenv_s(&the_sport_n_field_collection_will_get_removed_at_the_end_of_the_armory_pass_its_a_good_invest_trust, &len, L"PROGRAMFILES") == 0 && the_sport_n_field_collection_will_get_removed_at_the_end_of_the_armory_pass_its_a_good_invest_trust) {
        std::wstring path = the_sport_n_field_collection_will_get_removed_at_the_end_of_the_armory_pass_its_a_good_invest_trust;
        path += L"\\neofetch-win\\.neofetch-win";
        free(the_sport_n_field_collection_will_get_removed_at_the_end_of_the_armory_pass_its_a_good_invest_trust);

        if (GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES) {
            return path;
        }
    }

    if (_wdupenv_s(&the_sport_n_field_collection_will_get_removed_at_the_end_of_the_armory_pass_its_a_good_invest_trust, &len, L"USERPROFILE") == 0 && the_sport_n_field_collection_will_get_removed_at_the_end_of_the_armory_pass_its_a_good_invest_trust) {
        std::wstring path = the_sport_n_field_collection_will_get_removed_at_the_end_of_the_armory_pass_its_a_good_invest_trust;
        path += L"\\.neofetch-win";
        free(the_sport_n_field_collection_will_get_removed_at_the_end_of_the_armory_pass_its_a_good_invest_trust);
        return path;
    }

    return L".neofetch-win";
}

// note: make sure that the art is at least 19 lines long lols
std::vector<std::wstring> load_custom_art() {
    std::vector<std::wstring> custom_art;
    std::wstring config_path = get_art_config_path();

    std::ifstream file_utf8(config_path);
    if (file_utf8.is_open()) {
        std::string line;
        while (std::getline(file_utf8, line)) {
            if (line.empty() || line[0] != '#') {
                int size = MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, nullptr, 0);
                if (size > 0) {
                    std::wstring wide_line(size - 1, 0);
                    MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, &wide_line[0], size);
                    custom_art.push_back(wide_line);
                }
            }
        }
        file_utf8.close();
        return custom_art;
    }

    std::wifstream file(config_path);
    if (!file.is_open()) {
        return custom_art;
    }

    std::wstring line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] != L'#') {
            custom_art.push_back(line);
        }
    }

    file.close();
    return custom_art;
}
