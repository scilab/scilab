/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2011 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "registry.h"
#include "PATH_MAX.h"
#include "MALLOC.h"
#include "GetWindowsVersion.h"
#include "strdup_windows.h"
/*--------------------------------------------------------------------------*/
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
/*--------------------------------------------------------------------------*/

static BOOL WindowsOpenRegistry(char* _pstRoot, char* _pstKey, HKEY* _pKeyOut)
{
    DWORD OpensKeyOptions = 0;
    HKEY hKeyToOpen = NULL;

    hKeyToOpen = GetHkeyrootFromString(_pstRoot);
    if (hKeyToOpen == NULL)
    {
        return FALSE;
    }

#ifdef _WIN64 /* Scilab x64 on x64 windows */
    OpensKeyOptions = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY;
    if ( RegOpenKeyEx(hKeyToOpen, _pstKey, 0, OpensKeyOptions, _pKeyOut) != ERROR_SUCCESS)
    {
        OpensKeyOptions = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_32KEY;
        if ( RegOpenKeyEx(hKeyToOpen, _pstKey, 0, OpensKeyOptions, _pKeyOut) != ERROR_SUCCESS)
        {
            return FALSE;
        }
    }
#else
    if (IsWow64()) /* Scilab 32 bits on x64 windows */
    {
        OpensKeyOptions = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY;
        if ( RegOpenKeyEx(hKeyToOpen, _pstKey, 0, OpensKeyOptions, _pKeyOut) != ERROR_SUCCESS)
        {
            OpensKeyOptions = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_32KEY;
            if ( RegOpenKeyEx(hKeyToOpen, _pstKey, 0, OpensKeyOptions, _pKeyOut) != ERROR_SUCCESS)
            {
                return FALSE;
            }
        }
    }
    else /* Scilab 32 bits on windows 32 bits */
    {
        OpensKeyOptions = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE;
        if ( RegOpenKeyEx(hKeyToOpen, _pstKey, 0, OpensKeyOptions, _pKeyOut) != ERROR_SUCCESS)
        {
            return FALSE;
        }
    }
#endif
    return TRUE;
}

BOOL WindowsQueryRegistry(char *ParamIn1, char *ParamIn2, char *ParamIn3, char *ParamOut1, int *ParamOut2, BOOL *OuputIsREG_SZ)
{
    BOOL bOK = TRUE;
    HKEY key;
    DWORD type = 0;

    if (WindowsOpenRegistry(ParamIn1, ParamIn2, &key) == FALSE)
    {
        return FALSE;
    }

    if ( RegQueryValueEx(key, ParamIn3, NULL, &type, NULL, NULL) == ERROR_SUCCESS )
    {
        if ( (type == REG_EXPAND_SZ) || (type == REG_SZ) )
        {
            DWORD Length = PATH_MAX;
            char Line[PATH_MAX];
            if (RegQueryValueEx(key, ParamIn3, NULL, &type, (LPBYTE)&Line, &Length) == ERROR_SUCCESS )
            {
                wsprintf(ParamOut1, "%s", Line);
                *OuputIsREG_SZ = TRUE;
            }
        }
        else
        {
            DWORD size = 4;
            int Num = 0;
            if (RegQueryValueEx(key, ParamIn3, NULL, &type, (LPBYTE)&Num, &size) == ERROR_SUCCESS )
            {
                *ParamOut2 = Num;
                *OuputIsREG_SZ = FALSE;
            }
        }
    }
    else
    {
        bOK = FALSE;
    }

    RegCloseKey(key);

    return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL WindowsQueryRegistryValuesList(char *ParamIn1, char *ParamIn2, int dimMax, char **ListKeys)
{
    HKEY key;
    int i = 0;

    if (WindowsOpenRegistry(ParamIn1, ParamIn2, &key) == FALSE)
    {
        return FALSE;
    }

    for (i = 0; i < dimMax; i++)
    {
        TCHAR achKey[MAX_KEY_LENGTH];
        DWORD cbName = MAX_KEY_LENGTH;
        DWORD Type = 0;
        DWORD retCode = 0;

        retCode = RegEnumValue(key, i,
                               achKey,
                               &cbName,
                               NULL,
                               &Type,
                               NULL,
                               NULL);

        if (retCode != ERROR_SUCCESS)
        {
            RegCloseKey(key);
            return FALSE;
        }
        else
        {
            ListKeys[i] = strdup(achKey);
        }
    }

    RegCloseKey(key);

    return TRUE;
}

BOOL WindowsQueryRegistryKeysList(char *ParamIn1, char *ParamIn2, int dimMax, char **ListKeys)
{
    HKEY key;
    int i = 0;

    if (WindowsOpenRegistry(ParamIn1, ParamIn2, &key) == FALSE)
    {
        return FALSE;
    }

    for (i = 0; i < dimMax; i++)
    {
        TCHAR achKey[MAX_KEY_LENGTH];
        DWORD cbName = MAX_KEY_LENGTH;

        LONG Err = RegEnumKey(key, i, achKey, cbName);
        if (Err != ERROR_SUCCESS)
        {
            RegCloseKey(key);
            return FALSE;
        }
        else
        {
            ListKeys[i] = strdup(achKey);
        }
    }

    RegCloseKey(key);

    return TRUE;
}
/*--------------------------------------------------------------------------*/
HKEY GetHkeyrootFromString(char *string)
{
    if ( strcmp(string, "HKEY_CLASSES_ROOT") == 0 || strcmp(string, "HKCR") == 0)
    {
        return HKEY_CLASSES_ROOT;
    }

    if ( strcmp(string, "HKEY_CURRENT_USER") == 0 || strcmp(string, "HKCU") == 0 )
    {
        return HKEY_CURRENT_USER;
    }

    if ( strcmp(string, "HKEY_LOCAL_MACHINE") == 0 || strcmp(string, "HKLM") == 0 )
    {
        return HKEY_LOCAL_MACHINE;
    }

    if ( strcmp(string, "HKEY_USERS") == 0 || strcmp(string, "HKU") == 0 )
    {
        return HKEY_USERS;
    }

    if ( strcmp(string, "HKEY_DYN_DATA") == 0 || strcmp(string, "HKDD") == 0 )
    {
        return HKEY_DYN_DATA;
    }

    if ( strcmp(string, "HKEY_CURRENT_CONFIG") == 0 || strcmp(string, "HKCC") == 0 )
    {
        return HKEY_CURRENT_CONFIG;
    }

    return NULL;
}
/*--------------------------------------------------------------------------*/
static BOOL WindowsQueryRegistryNumberOfItemsInList(char *ParamIn1, char *ParamIn2, int* _piKeyNumber, int* _piValueNumber)
{
    BOOL bOK = TRUE;

    HKEY key;
    DWORD retCode = 0;

    TCHAR    achClass[PATH_MAX] = TEXT(""); // buffer for class name
    DWORD    cchClassName = PATH_MAX;       // size of class string
    DWORD    cSubKeys = 0;                  // number of subkeys
    DWORD    cbMaxSubKey = 0;               // longest subkey size
    DWORD    cchMaxClass = 0;               // longest class string
    DWORD    cValues = 0;                   // number of values for key
    DWORD    cchMaxValue = 0;               // longest value name
    DWORD    cbMaxValueData = 0;            // longest value data
    DWORD    cbSecurityDescriptor = 0;      // size of security descriptor
    FILETIME ftLastWriteTime;               // last write time

    if (WindowsOpenRegistry(ParamIn1, ParamIn2, &key) == FALSE)
    {
        return FALSE;
    }

    retCode = RegQueryInfoKey(
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

    if (retCode != ERROR_SUCCESS)
    {
        bOK = FALSE;
    }
    else
    {
        *_piValueNumber = cValues;
        *_piKeyNumber   = cSubKeys;
    }

    RegCloseKey(key);

    return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL WindowsQueryRegistryNumberOfValuesInList(char *ParamIn1, char *ParamIn2, int *Number)
{
    int iKey = 0;
    return WindowsQueryRegistryNumberOfItemsInList(ParamIn1, ParamIn2, &iKey, Number);
}
/*--------------------------------------------------------------------------*/
BOOL WindowsQueryRegistryNumberOfKeysInList(char *ParamIn1, char *ParamIn2, int *Number)
{
    int iValue = 0;
    return WindowsQueryRegistryNumberOfItemsInList(ParamIn1, ParamIn2, Number, &iValue);
}
/*--------------------------------------------------------------------------*/
