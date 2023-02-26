#pragma once

#include <Windows.h>

#include <string>
#include <memory>
#include <exception>

#include <assert.h>
#include <tchar.h>
#include <vector>
#include <stdexcept>
#include <system_error>

// this header was not written by me, many thanks to m4x1m1l14n on Github for making my life a lot easier (well, at least the part of my life involving regkey string queries)

namespace m4x1m1l14n
{
	namespace Registry
	{
		/*enum class ValueType
		{
			NUL,
			BOOLEAN,
			INT32,
			INT64,
			STRING,
			BINARY
		};*/

		enum class DesiredAccess : REGSAM
		{
			Delete = DELETE,
			ReadControl = READ_CONTROL,
			WriteDAC = WRITE_DAC,
			WriteOwner = WRITE_OWNER,
			AllAccess = KEY_ALL_ACCESS,
			CreateSubKey = KEY_CREATE_SUB_KEY,
			EnumerateSubKeys = KEY_ENUMERATE_SUB_KEYS,
			Execute = KEY_EXECUTE,
			Notify = KEY_NOTIFY,
			QueryValue = KEY_QUERY_VALUE,
			Read = KEY_READ,
			SetValue = KEY_SET_VALUE,
			Wow6432 = KEY_WOW64_32KEY,
			Wow6464 = KEY_WOW64_64KEY,
			Write = KEY_WRITE
		};

		DEFINE_ENUM_FLAG_OPERATORS(DesiredAccess);

		enum class CreateKeyOptions : DWORD
		{
			BackupRestore = REG_OPTION_BACKUP_RESTORE,
			NonVolatile = REG_OPTION_NON_VOLATILE,
			Volatile = REG_OPTION_VOLATILE
		};

		enum class NotifyFilter : DWORD
		{
			ChangeName = REG_NOTIFY_CHANGE_NAME,
			ChangeAttributes = REG_NOTIFY_CHANGE_ATTRIBUTES,
			ChangeLastSet = REG_NOTIFY_CHANGE_LAST_SET,
			ChangeSecurity = REG_NOTIFY_CHANGE_SECURITY,
			ThreadAgnostic = REG_NOTIFY_THREAD_AGNOSTIC
		};

		DEFINE_ENUM_FLAG_OPERATORS(NotifyFilter);

#if 0
		class RegistryValue
		{
		public:
			virtual Registry::ValueType GetType() const { return ValueType::NUL; }

			virtual bool IsNull() const { return true; }
			virtual bool IsNumber() const { return false; }
			virtual bool IsInt32() const { return false; }
			virtual bool IsInt64() const { return false; }
			virtual bool IsString() const { return false; }
			virtual bool IsBool() const { return false; }

			virtual bool GetBoolean() const { return false; }
			virtual long GetInt32() const { return 0; }
			virtual long long GetInt64() const { return 0; }
			virtual std::wstring GetString() const { return std::wstring(); }
			virtual std::shared_ptr<BYTE> GetBinary() const { return std::shared_ptr<BYTE>(); }
		};

		template <Registry::ValueType tag, typename T>
		class Value : public RegistryValue {
		protected:
			explicit Value(const T& value) : m_value(value), m_type(tag) {  }
			explicit Value(T&& value) : m_value(std::move(value)), m_type(tag) { }

		public:
			Registry::ValueType GetType() const override { return tag; }
			virtual bool IsNull() const override { return (m_type == Registry::ValueType::NUL); }
			virtual bool IsNumber() const override { return  (m_type == Registry::ValueType::INT32 || m_type == Registry::ValueType::INT64); }
			virtual bool IsInt32() const override { return  (m_type == Registry::ValueType::INT32); }
			virtual bool IsInt64() const override { return  (m_type == Registry::ValueType::INT64); }
			virtual bool IsString() const override { return  (m_type == Registry::ValueType::STRING); }
			virtual bool IsBool() const override { return (m_type == Registry::ValueType::BOOLEAN); }

		protected:
			T m_value;
			Registry::ValueType m_type;
		};

		class RegistryBoolean final : public Value<ValueType::BOOLEAN, bool> {
		public:
			RegistryBoolean(bool value) : Value(value) {}

			bool GetBoolean() const override { return m_value; }
		};

		class RegistryInt32 final : public Value<ValueType::INT32, long> {
		public:
			RegistryInt32(long value) : Value(value) {}

			long GetInt32() const override { return m_value; }
		};

		class RegistryInt64 final : public Value<ValueType::INT64, long long> {
		public:
			RegistryInt64(long long value) : Value(value) {}

			long long GetInt64() const override { return m_value; }
		};

		class RegistryString final : public Value<ValueType::STRING, std::wstring> {
		public:
			RegistryString(const std::wstring& value) : Value(value) {}

			std::wstring GetString() const override { return m_value; }
		};

		class RegistryBinary final : public Value<ValueType::BINARY, std::shared_ptr<BYTE>> {
		public:
			RegistryBinary(const std::shared_ptr<BYTE>& value) : Value(value) {}

			std::shared_ptr<BYTE> GetBinary() const override { return m_value; }
		};
#endif

		class RegistryKey;

		typedef std::shared_ptr<RegistryKey> RegistryKey_ptr;

		class RegistryKey
		{
		private:
			/// <summary>
			/// Deleted default constructor
			/// </summary>
			RegistryKey() = delete;

		public:
			RegistryKey(HKEY hKey)
				: m_hKey(hKey)
			{
				if (m_hKey == nullptr)
				{
					throw std::invalid_argument("Registry key handle cannot be nullptr");
				}
			}

			// Disable copy ctor & copy assignment operator
			RegistryKey(const RegistryKey& other) = delete;
			RegistryKey& operator=(RegistryKey& other) = delete;

			// TODO Implement move logic
			// RegistryKey(RegistryKey&& other);
			// RegistryKey& operator=(RegistryKey&& other);

			~RegistryKey()
			{
				if ((m_hKey != nullptr) && !(
					(m_hKey >= HKEY_CLASSES_ROOT) &&
#if (WINVER >= 0x0400)
					(m_hKey <= HKEY_CURRENT_USER_LOCAL_SETTINGS)
#else
					(m_hKey <= HKEY_PERFORMANCE_DATA)
#endif
					))
				{
					RegCloseKey(m_hKey);
				}
			}

			operator HKEY() const
			{
				return m_hKey;
			}

			/// <summary>
			///		Member method to open registry key on specified path, with specified access rights
			/// </summary>
			/// <param name="path">Relative path to subkey of this registry key</param>
			/// <param name="access>
			///		Desired access rights to open specified key.
			///		Default only read!
			///	</param>
			/// <exception></exception>
			RegistryKey_ptr Open(const std::wstring& path, DesiredAccess access = DesiredAccess::Read)
			{
				if (path.empty())
				{
					throw std::invalid_argument("Specified path to registry key cannot be empty");
				}

				HKEY hKey = nullptr;

				DWORD dwOptions = 0;

				LSTATUS lStatus = RegOpenKeyEx(m_hKey, path.c_str(), dwOptions, static_cast<REGSAM>(access), &hKey);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegOpenKeyEx() failed");
				}

				assert(hKey != nullptr);

				return std::make_shared<RegistryKey>(hKey);
			}

			/// <summary>
			///		Creates registry key on specified path
			/// </summary>
			/// <param name="path">Relative path to subkey to create</param>
			RegistryKey_ptr CreateVolatile(const std::wstring& path, DesiredAccess access = DesiredAccess::Read)
			{
				return Create(path, access, CreateKeyOptions::Volatile);
			}

			/// <summary>
			///		Creates registry key on specified path
			/// </summary>
			/// <param name="path">Relative path to subkey to create</param>
			/// <param name="access">Relative path to subkey to create</param>
			/// <param name="options">Relative path to subkey to create</param>
			RegistryKey_ptr Create(const std::wstring& path, DesiredAccess access = DesiredAccess::Read, CreateKeyOptions options = CreateKeyOptions::NonVolatile)
			{
				if (path.empty())
				{
					throw std::invalid_argument("Specified path to registry key cannot be empty");
				}

				HKEY hKey = nullptr;

				DWORD dwReserved = 0;
				LPTSTR lpClass = nullptr;

				LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr;

				LSTATUS lStatus =
					RegCreateKeyEx(
						m_hKey,
						path.c_str(),
						dwReserved,
						lpClass,
						static_cast<DWORD>(options),
						static_cast<REGSAM>(access),
						lpSecurityAttributes,
						&hKey,
						nullptr
					);

				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegOpenKeyEx() failed");
				}

				assert(hKey != nullptr);

				return std::make_shared<RegistryKey>(hKey);
			}

			void Delete()
			{
				LPTSTR lpSubKey = nullptr;

				LSTATUS lStatus = RegDeleteTree(m_hKey, lpSubKey);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegDeleteTree() failed");
				}
			}

			void Delete(const std::wstring& name)
			{
				LSTATUS lStatus = RegDeleteValue(m_hKey, name.c_str());
				// In case registry entry with specified name is not registry Value
				// RegDeleteValue() returns ERROR_FILE_NOT_FOUND, so we will try to
				// delete registry entry as if it is registry Key
				if (lStatus == ERROR_FILE_NOT_FOUND)
				{
					lStatus = RegDeleteTree(m_hKey, name.c_str());
				}

				if (lStatus != ERROR_SUCCESS && lStatus != ERROR_FILE_NOT_FOUND)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegDeleteTree() failed");
				}
			}

			void Flush()
			{
				LSTATUS lStatus = RegFlushKey(m_hKey);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegFlushKey() failed");
				}
			}

			void Save(const std::wstring& file)
			{
				LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr;

				LSTATUS lStatus = RegSaveKey(m_hKey, file.c_str(), lpSecurityAttributes);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegFlushKey() failed");
				}
			}



			/// <summary>
			///		Checks whether specified subkey exists or not
			/// </summary>
			/// <param name="path">Subkey relative path to be checked for existence</param>
			bool HasKey(const std::wstring& path)
			{
				if (path.empty())
				{
					throw std::invalid_argument("Specified path to registry key cannot be empty");
				}

				auto hasKey = false;

				HKEY hKey = nullptr;

				LSTATUS lStatus = RegOpenKeyEx(m_hKey, path.c_str(), 0, KEY_READ, &hKey);

				if (lStatus == ERROR_SUCCESS)
				{
					assert(hKey != nullptr);
					RegCloseKey(hKey);

					hasKey = true;
				}
				else if (lStatus != ERROR_FILE_NOT_FOUND)
				{
					assert(hKey == nullptr);

					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegOpenKeyEx() failed");
				}

				return hasKey;
			}

			// For backward compatibility only
			bool Exists(const std::wstring& path)
			{
				return HasKey(path);
			}

			bool HasValue(const std::wstring& name)
			{
				if (name.empty())
				{
					throw std::invalid_argument("Value name cannot be empty");
				}

				auto hasValue = false;

				LSTATUS lStatus = RegQueryValueEx(m_hKey, name.c_str(), nullptr, nullptr, nullptr, nullptr);
				if (lStatus == ERROR_SUCCESS)
				{
					hasValue = true;
				}
				else if (lStatus != ERROR_FILE_NOT_FOUND)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegQueryValueEx() failed");
				}

				return hasValue;
			}

			bool GetBoolean(const std::wstring& name)
			{
				DWORD dwType = 0;
				DWORD dwData = 0;
				DWORD cbData = sizeof(dwData);

				LPDWORD lpReserver = nullptr;

				LSTATUS lStatus = RegQueryValueEx(m_hKey, name.c_str(), lpReserver, &dwType, reinterpret_cast<LPBYTE>(&dwData), &cbData);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegQueryValueEx() failed");
				}

				if (dwType != REG_DWORD && dwType != REG_QWORD)
				{
					throw std::runtime_error("Wrong registry value type " + std::to_string(dwType) + " for boolean value.");
				}

				return (dwData == 0) ? false : true;
			}

			// Default registry value
			bool GetBoolean()
			{
				return GetBoolean(L"");
			}

			void SetBoolean(const std::wstring& name, bool value)
			{
				DWORD dwValue = value ? 1 : 0;
				DWORD cbData = sizeof(dwValue);

				LSTATUS lStatus = RegSetValueEx(m_hKey, name.c_str(), 0, REG_DWORD, reinterpret_cast<const LPBYTE>(&dwValue), cbData);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegSetValueEx() failed");
				}
			}

			void SetBoolean(bool value)
			{
				SetBoolean(L"", value);
			}

			long GetInt32(const std::wstring& name)
			{
				long lData = 0;
				DWORD cbData = sizeof(lData);

				LSTATUS lStatus = RegQueryValueEx(m_hKey, name.c_str(), nullptr, nullptr, reinterpret_cast<LPBYTE>(&lData), &cbData);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegQueryValueEx() failed");
				}

				return lData;
			}

			long GetInt32()
			{
				return GetInt32(L"");
			}

			unsigned long GetUInt32(const std::wstring& name)
			{
				return static_cast<unsigned long>(GetInt32(name));
			}

			unsigned long GetUInt32()
			{
				return GetUInt32(L"");
			}

			void SetInt32(const std::wstring& name, long value)
			{
				DWORD cbData = sizeof(value);

				LSTATUS lStatus = RegSetValueEx(m_hKey, name.c_str(), 0, REG_DWORD, reinterpret_cast<const LPBYTE>(&value), cbData);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegSetValueEx() failed");
				}
			}

			void SetInt32(long value)
			{
				return SetInt32(L"", value);
			}

			void SetUInt32(const std::wstring& name, unsigned long value)
			{
				return SetInt32(name, static_cast<long>(value));
			}

			void SetUInt32(unsigned long value)
			{
				return SetUInt32(L"", value);
			}

			long long GetInt64(const std::wstring& name)
			{
				long long llData = 0;
				DWORD cbData = sizeof(llData);

				LSTATUS lStatus = RegQueryValueEx(m_hKey, name.c_str(), nullptr, nullptr, reinterpret_cast<LPBYTE>(&llData), &cbData);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegQueryValueEx() failed");
				}

				return llData;
			}

			long long GetInt64()
			{
				return GetInt64(L"");
			}

			unsigned long long GetUInt64(const std::wstring& name)
			{
				return static_cast<unsigned long long>(GetInt64(name));
			}

			unsigned long long GetUInt64()
			{
				return static_cast<unsigned long long>(GetUInt64(L""));
			}

			void SetInt64(const std::wstring& name, long long value)
			{
				DWORD cbData = sizeof(value);

				LSTATUS lStatus = RegSetValueEx(m_hKey, name.c_str(), 0, REG_QWORD, reinterpret_cast<const LPBYTE>(&value), cbData);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegSetValueEx() failed");
				}
			}

			void SetInt64(long long value)
			{
				return SetInt64(L"", value);
			}

			void SetUInt64(const std::wstring& name, unsigned long long value)
			{
				SetInt64(name, static_cast<long long>(value));
			}

			void SetUInt64(unsigned long long value)
			{
				SetUInt64(L"", value);
			}

			std::wstring GetString(const std::wstring& name)
			{
				DWORD cbData = 0;
				DWORD dwType = 0;

				DWORD dwFlags = RRF_RT_REG_EXPAND_SZ | RRF_NOEXPAND | RRF_RT_REG_SZ;

				LSTATUS lStatus = RegGetValue(m_hKey, nullptr, name.c_str(), dwFlags, &dwType, nullptr, &cbData);
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegGetValue() failed");
				}

				if (dwType != REG_SZ && dwType != REG_EXPAND_SZ) // ???
				{
					throw std::runtime_error("Wrong registry value type " + std::to_string(dwType) + " for string value.");
				}

				std::wstring value;

				if (cbData)
				{
					assert((cbData % sizeof(TCHAR)) != 1);

					auto data = new TCHAR[cbData / sizeof(TCHAR)];
					assert(data != nullptr);

					lStatus = RegGetValue(m_hKey, nullptr, name.c_str(), dwFlags, &dwType, reinterpret_cast<LPBYTE>(data), &cbData);

					std::exception_ptr pex;

					if (lStatus == ERROR_SUCCESS)
					{
						//assert(data[cbData / sizeof(TCHAR) - 1] == _T('\0'));

						value = std::wstring(data);
					}
					else
					{
						auto ec = std::error_code(lStatus, std::system_category());

						pex = std::make_exception_ptr(std::system_error(ec, "RegGetValue() failed"));
					}

					delete[] data;

					if (pex)
					{
						std::rethrow_exception(pex);
					}
				}

				return value;
			}

			std::wstring GetString()
			{
				return GetString(L"");
			}

			void SetString(const std::wstring& name, const std::wstring& value)
			{
				auto cbData = static_cast<DWORD>(value.length());

				LSTATUS lStatus = RegSetValueEx(m_hKey, name.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(value.c_str()), cbData * sizeof(TCHAR));
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegSetValueEx() failed");
				}
			}

			void SetString(const std::wstring& value)
			{
				SetString(L"", value);
			}

			/// <summary>
			///	Create registry value with specified name of type REG_EXPAND_SZ within this registry key
			/// </summary>
			/// <param name="name">Name of registry value (Empty string for default key value)</param>
			/// <param name="value">Value to be set</param>
			void SetExpandString(const std::wstring& name, const std::wstring& value)
			{
				auto cbData = static_cast<DWORD>(value.length());

				LSTATUS lStatus = RegSetValueEx(m_hKey, name.c_str(), 0, REG_EXPAND_SZ, reinterpret_cast<const BYTE*>(value.c_str()), cbData * sizeof(TCHAR));
				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegSetValueEx() failed");
				}
			}

			/// <summary>
			///	Create default registry value of type REG_EXPAND_SZ within this registry key
			/// </summary>
			/// <param name="value">Value to be set</param>
			void SetExpandString(const std::wstring& value)
			{
				SetExpandString(L"", value);
			}

			template <typename __Function>
			void EnumerateSubKeys(const __Function& callback)
			{
				DWORD dwSubKeys = 0;
				DWORD dwLongestSubKeyLen = 0;

				LSTATUS lStatus = RegQueryInfoKey
				(
					m_hKey,					// Key handle
					nullptr,				// Buffer for registry ked class name
					nullptr,				// Size of class string
					nullptr,				// Reserved
					&dwSubKeys,				// Number of key subkeys (this is what we want)
					&dwLongestSubKeyLen,	// Longest subkey size
					nullptr,				// Longest class string
					nullptr,				// number of values for this key
					nullptr,				// Longest value name
					nullptr,				// Longest value data
					nullptr,				// Security descriptor
					nullptr					// Last key write time
				);

				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegQueryInfoKey() failed");
				}

				// Add space fot terminating null character
				++dwLongestSubKeyLen;

				std::exception_ptr pex;

				auto pszName = reinterpret_cast<TCHAR*>(LocalAlloc(LMEM_FIXED, dwLongestSubKeyLen * sizeof(TCHAR)));
				assert(pszName != nullptr);

				for (DWORD i = 0; i < dwSubKeys; ++i)
				{
					DWORD dwLen = dwLongestSubKeyLen;

					lStatus = RegEnumKeyEx
					(
						m_hKey,			// Key handle
						i,				// Subkey index
						pszName,		// Subkey name buffer
						&dwLen,			// Subkey name string length
						nullptr,		// Reserved
						nullptr,		// Subkey class buffer
						nullptr,		// Subkey class string length
						nullptr			// Last subkey write time
					);

					if (lStatus != ERROR_SUCCESS)
					{
						auto ec = std::error_code(lStatus, std::system_category());

						pex = std::make_exception_ptr(
							std::system_error(ec, "RegQueryInfoKey() failed")
						);

						break;
					}

					std::wstring subKeyName(pszName, dwLen);

					// Catch possible exception thrown by lambda callback
					try
					{
						if (!callback(subKeyName))
						{
							// Break loop when callback returns false
							break;
						}
					}
					catch (const std::exception&)
					{
						pex = std::current_exception();

						break;
					}
				}

				LocalFree(pszName);

				if (pex)
				{
					std::rethrow_exception(pex);
				}
			}

			/// <summary>
			///	Notifies the caller about changes to the attributes or contents of a specified registry key.
			/// </summary>
			/// <param name="watchSubtree">
			///	If this parameter is true, the function reports changes in the specified key and its subkeys.
			///	Otherwise, the function reports changes only in the specified key.
			/// Default is false.
			/// </param>
			/// <param name="notifyFilter">
			///	A value that indicates the changes that should be reported.
			/// Default is ChangeName and ChangeAttributes.
			/// </param>
			void Notify(bool watchSubtree = false, NotifyFilter notifyFilter = NotifyFilter::ChangeName | NotifyFilter::ChangeAttributes)
			{
				HANDLE hEvent = nullptr;
				BOOL fAsynchronous = FALSE;

				LSTATUS lStatus = RegNotifyChangeKeyValue
				(
					m_hKey,
					watchSubtree ? TRUE : FALSE,
					static_cast<DWORD>(notifyFilter),
					hEvent,
					fAsynchronous
				);

				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegNotifyChangeKeyValue() failed");
				}
			}

			/// <summary>
			///	Notifies the caller about changes to the attributes or contents of a specified registry key asynchonously via provided event object.
			/// </summary>
			/// <param name="hEvent">
			///	A handle to an event. Call to this function returns immediately and changes are reported by signaling this event.
			/// </param>
			/// <param name="watchSubtree">
			///	If this parameter is true, the function reports changes in the specified key and its subkeys.
			///	Otherwise, the function reports changes only in the specified key.
			/// Default is false.
			/// </param>
			/// <param name="notifyFilter">
			///	A value that indicates the changes that should be reported.
			/// Default is ChangeName and ChangeAttributes.
			/// </param>
			void NotifyAsync(HANDLE hEvent, bool watchSubtree = false, NotifyFilter notifyFilter = NotifyFilter::ChangeName | NotifyFilter::ChangeAttributes | NotifyFilter::ChangeLastSet)
			{
				if (hEvent == nullptr)
				{
					throw std::invalid_argument("Event handle cannot be null");
				}

				BOOL fAsynchronous = TRUE;

				LSTATUS lStatus = RegNotifyChangeKeyValue
				(
					m_hKey,
					watchSubtree ? TRUE : FALSE,
					static_cast<DWORD>(notifyFilter),
					hEvent,
					fAsynchronous
				);

				if (lStatus != ERROR_SUCCESS)
				{
					auto ec = std::error_code(lStatus, std::system_category());

					throw std::system_error(ec, "RegNotifyChangeKeyValue() failed");
				}
			}

#if 0
			std::shared_ptr<BYTE> GetBinary(const std::wstring& name, size_t len)
			{
				DWORD cbData = DWORD(len);
				BYTE* data = new BYTE[len];
				std::shared_ptr<BYTE> ret;

				if (data != nullptr)
				{
					LSTATUS lStatus = RegQueryValueEx(m_hKey, name.c_str(), nullptr, nullptr, data, &cbData);
					if (lStatus == ERROR_SUCCESS)
					{
						ret = std::shared_ptr<BYTE>(data, std::default_delete<BYTE[]>());
					}
				}

				return ret;
			}

			bool SetBinary(const std::wstring& name, const BYTE* pData, size_t len)
			{
				DWORD cbData = DWORD(len);

				LSTATUS lStatus = RegSetValueEx(m_hKey, name.c_str(), 0, REG_BINARY, pData, DWORD(len));

				return (lStatus == ERROR_SUCCESS);
			}

			RegistryValue GetValue(const std::wstring& name)
			{
				assert(m_hKey != nullptr);
				DWORD dwType = 0;
				DWORD cbData = 0;

				RegistryValue ret;

				LSTATUS err = RegQueryValueEx(m_hKey, name.c_str(), nullptr, &dwType, nullptr, &cbData);
				if (err == ERROR_SUCCESS)
				{
					switch (dwType)
					{
					case REG_DWORD:
					{
						long value = 0;
						cbData = sizeof(value);

						if (RegQueryValueEx(m_hKey, name.c_str(), nullptr, nullptr, (LPBYTE)&value, &cbData) == ERROR_SUCCESS)
						{
							ret = RegistryInt32(value);
						}
					}
					break;

					case REG_QWORD:
					{
						long long value = 0;
						cbData = sizeof(value);

						if (RegQueryValueEx(m_hKey, name.c_str(), nullptr, nullptr, (LPBYTE)&value, &cbData) == ERROR_SUCCESS)
						{
							ret = RegistryInt64(value);
						}
					}
					break;

					case REG_SZ:
					{
						WCHAR* data = new WCHAR[cbData];
						if (data != nullptr)
						{
							LSTATUS lStatus = RegQueryValueEx(m_hKey, name.c_str(), nullptr, nullptr, (LPBYTE)data, &cbData);
							if (lStatus == ERROR_SUCCESS)
							{
								std::wstring s(data, cbData);
								ret = RegistryString(s);
							}

							delete[] data;
						}
					}
					break;

					case REG_BINARY:
					{
						BYTE* data = new BYTE[cbData];

						if (data != nullptr)
						{
							LSTATUS lStatus = RegQueryValueEx(m_hKey, name.c_str(), nullptr, nullptr, data, &cbData);
							if (lStatus == ERROR_SUCCESS)
							{
								ret = RegistryBinary(
									std::shared_ptr<BYTE>(
										data,
										std::default_delete<BYTE[]>()
										)
								);
							}
						}
					}
					break;

					default:
						break;
					}
				}

				return ret;
			}

			void SetValue(const std::wstring& valueName, const RegistryValue& value)
			{
				switch (value.GetType())
				{
				case Registry::ValueType::BOOLEAN: { return this->SetBoolean(valueName, value.GetBoolean()); } break;
				case Registry::ValueType::INT32: { return this->SetInt32(valueName, value.GetInt32()); } break;
				case Registry::ValueType::INT64: { return this->SetInt64(valueName, value.GetInt64()); } break;
				case Registry::ValueType::STRING: { return this->SetString(valueName, value.GetString()); } break;
					//case Registry::Type::BINARY: { return this->SetBinary(valueName, *value.GetBinary(), ); }
				}
			}
#endif

#if 0
			std::vector<std::wstring> GetSubKeys()
			{
				std::vector<std::wstring> vecRet;
				DWORD    dwSubKeys = 0;
				DWORD	dwMaxSubKeyLen = 0;
				LSTATUS lStatus = RegQueryInfoKey(m_hKey, NULL, NULL, NULL, &dwSubKeys, NULL, NULL, NULL, &dwMaxSubKeyLen, NULL, NULL, NULL);
				if (lStatus == ERROR_SUCCESS && dwSubKeys > 0)
				{

					for (DWORD i = 0; i < dwSubKeys; i++)
					{
						WCHAR   pwszName[255];
						DWORD dwNameLen = 255;
						lStatus = RegEnumKeyEx(m_hKey, i, pwszName, &dwNameLen, NULL, NULL, NULL, NULL);
						if (lStatus == ERROR_SUCCESS)
						{
							vecRet.push_back(std::wstring(pwszName, dwNameLen));
						}
					}
				}

				return vecRet;
			}
#endif

		private:
			HKEY m_hKey;
		};

		extern RegistryKey_ptr ClassesRoot;
		extern RegistryKey_ptr CurrentUser;
		extern RegistryKey_ptr LocalMachine;
		extern RegistryKey_ptr Users;
		extern RegistryKey_ptr CurrentConfig;
	}
}