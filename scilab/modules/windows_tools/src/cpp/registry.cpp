/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2011 - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
#include <Windows.h>
#include "registry.hxx"

extern "C"
{
#include "PATH_MAX.h"
#include "sci_malloc.h"
#include "GetWindowsVersion.h"
#include "os_string.h"
#include "charEncoding.h"
}

/*--------------------------------------------------------------------------*/
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
/*--------------------------------------------------------------------------*/

static bool WindowsOpenRegistry(const std::wstring& root, const std::wstring& key, HKEY* out)
{
    DWORD OpensKeyOptions = 0;
    HKEY hKeyToOpen = GetHkeyrootFromString(root);
    if (hKeyToOpen == NULL)
    {
        return false;
    }

#ifdef _WIN64 /* Scilab x64 on x64 windows */
    OpensKeyOptions = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY;
    if ( RegOpenKeyExW(hKeyToOpen, key.data(), 0, OpensKeyOptions, out) != ERROR_SUCCESS)
    {
        OpensKeyOptions = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_32KEY;
        if (RegOpenKeyExW(hKeyToOpen, key.data(), 0, OpensKeyOptions, out) != ERROR_SUCCESS)
        {
            return false;
        }
    }
#else
    if (IsWow64()) /* Scilab 32 bits on x64 windows */
    {
        OpensKeyOptions = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY;
        if ( RegOpenKeyExW(hKeyToOpen, key.data(), 0, OpensKeyOptions, out) != ERROR_SUCCESS)
        {
            OpensKeyOptions = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_32KEY;
            if ( RegOpenKeyExW(hKeyToOpen, key.data(), 0, OpensKeyOptions, out) != ERROR_SUCCESS)
            {
                return false;
            }
        }
    }
    else /* Scilab 32 bits on windows 32 bits */
    {
        OpensKeyOptions = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE;
        if ( RegOpenKeyExW(hKeyToOpen, key.data(), 0, OpensKeyOptions, out) != ERROR_SUCCESS)
        {
            return false;
        }
    }
#endif
    return true;
}

bool WindowsQueryRegistry(const std::wstring& param1, const std::wstring& param2, const std::wstring& param3, std::wstring& out1, int& out2, bool& isStringKey)
{
    bool res = true;
    HKEY key;
    DWORD type = 0;

    if (WindowsOpenRegistry(param1, param2, &key) == FALSE)
    {
        return false;
    }

    if (RegQueryValueExW(key, param3.data(), NULL, &type, NULL, NULL) == ERROR_SUCCESS)
    {
        if ( (type == REG_EXPAND_SZ) || (type == REG_SZ) )
        {
            DWORD Length = PATH_MAX;
            wchar_t Line[PATH_MAX];
            if (RegQueryValueExW(key, param3.data(), NULL, &type, (LPBYTE)&Line, &Length) == ERROR_SUCCESS )
            {
                out1 = Line;
                isStringKey = true;
            }
        }
        else
        {
            DWORD size = 4;
            int Num = 0;
            if (RegQueryValueExW(key, param3.data(), NULL, &type, (LPBYTE)&Num, &size) == ERROR_SUCCESS )
            {
                out2 = Num;
                isStringKey = false;
            }
        }
    }
    else
    {
        res = false;
    }

    RegCloseKey(key);

    return res;
}
/*--------------------------------------------------------------------------*/
bool WindowsQueryRegistryValuesList(const std::wstring& param1, const std::wstring& param2, int dimMax, std::vector<std::wstring>& ListKeys)
{
    HKEY key;

    if (WindowsOpenRegistry(param1, param2, &key) == false)
    {
        return false;
    }

    for (int i = 0; i < dimMax; ++i)
    {
        wchar_t achKey[MAX_KEY_LENGTH];
        DWORD cbName = MAX_KEY_LENGTH;
        DWORD Type = 0;

        if (RegEnumValueW(key, i, achKey, &cbName, NULL, &Type, NULL, NULL) != ERROR_SUCCESS)
        {
            RegCloseKey(key);
            return false;
        }
        else
        {
            ListKeys.push_back(achKey);
        }
    }

    RegCloseKey(key);

    return true;
}

bool WindowsQueryRegistryKeysList(const std::wstring& param1, const std::wstring& param2, int dimMax, std::vector<std::wstring>& ListKeys)
{
    HKEY key;

    if (WindowsOpenRegistry(param1, param2, &key) == false)
    {
        return false;
    }

    for (int i = 0; i < dimMax; ++i)
    {
        wchar_t achKey[MAX_KEY_LENGTH];
        DWORD cbName = MAX_KEY_LENGTH;

        if (RegEnumKeyW(key, i, achKey, cbName) != ERROR_SUCCESS)
        {
            RegCloseKey(key);
            return false;
        }
        else
        {
            ListKeys.push_back(achKey);
        }
    }

    RegCloseKey(key);

    return true;
}
/*--------------------------------------------------------------------------*/
HKEY GetHkeyrootFromString(const std::wstring& param)
{
    if (param == L"HKEY_CLASSES_ROOT" || param == L"HKCR")
    {
        return HKEY_CLASSES_ROOT;
    }

    if (param == L"HKEY_CURRENT_USER" || param == L"HKCU")
    {
        return HKEY_CURRENT_USER;
    }

    if (param == L"HKEY_LOCAL_MACHINE" || param == L"HKLM")
    {
        return HKEY_LOCAL_MACHINE;
    }

    if (param == L"HKEY_USERS" || param == L"HKU")
    {
        return HKEY_USERS;
    }

    if (param == L"HKEY_DYN_DATA" || param == L"HKDD")
    {
        return HKEY_DYN_DATA;
    }

    if (param == L"HKEY_CURRENT_CONFIG" || param == L"HKCC")
    {
        return HKEY_CURRENT_CONFIG;
    }

    return NULL;
}
/*--------------------------------------------------------------------------*/
static bool WindowsQueryRegistryNumberOfItemsInList(const std::wstring& param1, const std::wstring& param2, int& _key, int& _value)
{
    HKEY        key;
    TCHAR       achClass[PATH_MAX] = TEXT(""); // buffer for class name
    DWORD       cchClassName = PATH_MAX;       // size of class string
    DWORD       cSubKeys = 0;                  // number of subkeys
    DWORD       cbMaxSubKey = 0;               // longest subkey size
    DWORD       cchMaxClass = 0;               // longest class string
    DWORD       cValues = 0;                   // number of values for key
    DWORD       cchMaxValue = 0;               // longest value name
    DWORD       cbMaxValueData = 0;            // longest value data
    DWORD       cbSecurityDescriptor = 0;      // size of security descriptor
    FILETIME    ftLastWriteTime;               // last write time

    if (WindowsOpenRegistry(param1, param2, &key) == false)
    {
        return false;
    }

    DWORD retCode = RegQueryInfoKey(
                        key,                      // key handle
                        achClass,                 // buffer for class name
                        &cchClassName,            // size of class string
                        NULL,                     // reserved
                        &cSubKeys,                // number of subkeys
                        &cbMaxSubKey,             // longest subkey size
                        &cchMaxClass,             // longest class string
                        &cValues,                 // number of values for this key
                        &cchMaxValue,             // longest value name
                        &cbMaxValueData,          // longest value data
                        &cbSecurityDescriptor,    // security descriptor
                        &ftLastWriteTime);        // last write time

    RegCloseKey(key);
    if (retCode != ERROR_SUCCESS)
    {
        return false;
    }

    _value = cValues;
    _key = cSubKeys;

    return true;
}
/*--------------------------------------------------------------------------*/
bool WindowsQueryRegistryNumberOfValuesInList(const std::wstring& param1, const std::wstring& param2, int& number)
{
    int key = 0;
    return WindowsQueryRegistryNumberOfItemsInList(param1, param2, key, number);
}
/*--------------------------------------------------------------------------*/
bool WindowsQueryRegistryNumberOfKeysInList(const std::wstring& param1, const std::wstring& param2, int& number)
{
    int value = 0;
    return WindowsQueryRegistryNumberOfItemsInList(param1, param2, number, value);
}
/*--------------------------------------------------------------------------*/
