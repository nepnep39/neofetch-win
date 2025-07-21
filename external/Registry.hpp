#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <memory>
#include <system_error>

// this header was not written by me, many thanks to m4x1m1l14n on Github for making my life a lot easier (well, at least the part of my life involving regkey string queries)
// https://github.com/m4x1m1l14n/Registry

namespace m4x1m1l14n
{
    namespace Registry
    {
        enum class DesiredAccess : REGSAM
        {
            Read = KEY_READ
        };

        class RegistryKey;
        typedef std::shared_ptr<RegistryKey> RegistryKey_ptr;

        class RegistryKey
        {
        private:
            RegistryKey() = delete;

        public:
            RegistryKey(HKEY hKey) : m_hKey(hKey)
            {
                if (m_hKey == nullptr)
                {
                    throw std::invalid_argument("Registry key handle cannot be nullptr");
                }
            }

            // no copy for u
            RegistryKey(const RegistryKey& other) = delete;
            RegistryKey& operator=(RegistryKey& other) = delete;

            ~RegistryKey()
            {
                if ((m_hKey != nullptr) && !(
                    (m_hKey >= HKEY_CLASSES_ROOT) &&
                    (m_hKey <= HKEY_CURRENT_USER_LOCAL_SETTINGS)))
                {
                    RegCloseKey(m_hKey);
                }
            }

            // only the good stuff, everything else = bloat
            RegistryKey_ptr Open(const std::wstring& path, DesiredAccess access = DesiredAccess::Read)
            {
                if (path.empty())
                {
                    throw std::invalid_argument("Specified path to registry key cannot be empty");
                }

                HKEY hKey = nullptr;
                LSTATUS lStatus = RegOpenKeyExW(m_hKey, path.c_str(), 0, static_cast<REGSAM>(access), &hKey);

                if (lStatus != ERROR_SUCCESS)
                {
                    auto ec = std::error_code(lStatus, std::system_category());
                    throw std::system_error(ec, "RegOpenKeyEx() failed");
                }

                return std::make_shared<RegistryKey>(hKey);
            }

            std::wstring GetString(const std::wstring& name)
            {
                DWORD cbData = 0;
                DWORD dwType = 0;
                DWORD dwFlags = RRF_RT_REG_EXPAND_SZ | RRF_NOEXPAND | RRF_RT_REG_SZ;

                LSTATUS lStatus = RegGetValueW(m_hKey, nullptr, name.c_str(), dwFlags, &dwType, nullptr, &cbData);
                if (lStatus != ERROR_SUCCESS)
                {
                    auto ec = std::error_code(lStatus, std::system_category());
                    throw std::system_error(ec, "RegGetValue() failed");
                }

                if (dwType != REG_SZ && dwType != REG_EXPAND_SZ)
                {
                    throw std::runtime_error("Wrong registry value type for string value.");
                }

                std::wstring value;
                if (cbData)
                {
                    auto data = new wchar_t[cbData / sizeof(wchar_t)];
                    lStatus = RegGetValueW(m_hKey, nullptr, name.c_str(), dwFlags, &dwType, reinterpret_cast<LPBYTE>(data), &cbData);

                    if (lStatus == ERROR_SUCCESS)
                    {
                        value = std::wstring(data);
                    }
                    else
                    {
                        auto ec = std::error_code(lStatus, std::system_category());
                        delete[] data;
                        throw std::system_error(ec, "RegGetValue() failed");
                    }
                    delete[] data;
                }
                return value;
            }

        private:
            HKEY m_hKey;
        };

        // we only need this one lol
        extern RegistryKey_ptr LocalMachine;
    }
}
