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
#include "win_mem_alloc.h"
#include "GetWindowsVersion.h"
#include "strdup_windows.h"
/*--------------------------------------------------------------------------*/
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
/*--------------------------------------------------------------------------*/
BOOL WindowsQueryRegistry(char *ParamIn1, char *ParamIn2, char *ParamIn3, char *ParamOut1, int *ParamOut2, BOOL *OuputIsREG_SZ)
{
    BOOL bOK = TRUE;
    HKEY key;
    HKEY hKeyToOpen = NULL;
    DWORD OpensKeyOptions = 0;
    DWORD type = 0;

    hKeyToOpen = GetHkeyrootFromString(ParamIn1);

#ifdef _WIN64 /* Scilab x64 on x64 windows */
    OpensKeyOptions = KEY_QUERY_VALUE | KEY_WOW64_64KEY;
    if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &key) != ERROR_SUCCESS)
    {
        OpensKeyOptions = KEY_QUERY_VALUE | KEY_WOW64_32KEY;
        if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &key) != ERROR_SUCCESS) return FALSE;
    }
#else
    if (IsWow64()) /* Scilab 32 bits on x64 windows */
    {
        OpensKeyOptions = KEY_QUERY_VALUE | KEY_WOW64_64KEY;
        if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &key) != ERROR_SUCCESS)
        {
            OpensKeyOptions = KEY_QUERY_VALUE | KEY_WOW64_32KEY;
            if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &key) != ERROR_SUCCESS) return FALSE;
        }
    }
    else /* Scilab 32 bits on windows 32 bits */
    {
        OpensKeyOptions = KEY_QUERY_VALUE;
        if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &key) != ERROR_SUCCESS) return FALSE;
    }
#endif

    if ( RegQueryValueEx(key, ParamIn3, NULL, &type, NULL, NULL) == ERROR_SUCCESS )
    {
        if ( (type == REG_EXPAND_SZ) || (type == REG_SZ) )
        {
            DWORD Length = PATH_MAX;
            char Line[PATH_MAX];
            if (RegQueryValueEx(key, ParamIn3, NULL, &type, (LPBYTE)&Line, &Length) == ERROR_SUCCESS )
            {
                wsprintf(ParamOut1,"%s",Line);
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
BOOL WindowsQueryRegistryList(char *ParamIn1, char *ParamIn2, int dimMax, char **ListKeys)
{
    BOOL bOK = TRUE;
    HKEY key;
    HKEY hKeyToOpen = NULL;
    DWORD OpensKeyOptions = 0;
    int i = 0;

    hKeyToOpen = GetHkeyrootFromString(ParamIn1);

#ifdef _WIN64 /* Scilab x64 on x64 windows */
    OpensKeyOptions = KEY_READ  | KEY_WOW64_64KEY;
    if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &key) != ERROR_SUCCESS )
    {
        OpensKeyOptions = KEY_READ  | KEY_WOW64_32KEY;
        if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &key) != ERROR_SUCCESS ) return FALSE;
    }
#else
    if (IsWow64())  /* Scilab 32 bits on x64 windows */
    {
        OpensKeyOptions = KEY_READ  | KEY_WOW64_64KEY;
        if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &key) != ERROR_SUCCESS )
        {
            OpensKeyOptions = KEY_READ  | KEY_WOW64_32KEY;
            if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &key) != ERROR_SUCCESS ) return FALSE;
        }
    }
    else /* Scilab 32 bits on windows 32 bits */
    {
        OpensKeyOptions = KEY_READ ;
        if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &key) != ERROR_SUCCESS ) return FALSE;
    }

#endif

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
            bOK = FALSE;
            RegCloseKey(key);
            return bOK;
        }
        else
        {
            ListKeys[i] = strdup(achKey);
        }
    }

    RegCloseKey(key);

    return bOK;
}
/*--------------------------------------------------------------------------*/
HKEY GetHkeyrootFromString(char *string)
{
    HKEY hkey = NULL;

    if ( strcmp(string,"HKEY_CLASSES_ROOT") == 0 )  hkey = HKEY_CLASSES_ROOT;
    if ( strcmp(string,"HKEY_CURRENT_USER") == 0 )  hkey = HKEY_CURRENT_USER;
    if ( strcmp(string,"HKEY_LOCAL_MACHINE") == 0 ) hkey = HKEY_LOCAL_MACHINE;
    if ( strcmp(string,"HKEY_USERS") == 0 )         hkey = HKEY_USERS;
    if ( strcmp(string,"HKEY_DYN_DATA") == 0 )      hkey = HKEY_DYN_DATA;
    if ( strcmp(string,"HKEY_CURRENT_CONFIG") == 0 )hkey = HKEY_CURRENT_CONFIG;

    return hkey;
}
/*--------------------------------------------------------------------------*/
BOOL WindowsQueryRegistryNumberOfElementsInList(char *ParamIn1,char *ParamIn2,int *Number)
{
    BOOL bOK = TRUE;

    HKEY hKeyToOpen;
    HKEY hTestKey;
    DWORD OpensKeyOptions = 0;

    DWORD retCode = 0;

    TCHAR    achClass[PATH_MAX] = TEXT("");  // buffer for class name
    DWORD    cchClassName = PATH_MAX;    // size of class string
    DWORD    cSubKeys = 0;               // number of subkeys
    DWORD    cbMaxSubKey = 0;            // longest subkey size
    DWORD    cchMaxClass = 0;            // longest class string
    DWORD    cValues = 0;                // number of values for key
    DWORD    cchMaxValue = 0;            // longest value name
    DWORD    cbMaxValueData = 0;         // longest value data
    DWORD    cbSecurityDescriptor = 0;  // size of security descriptor
    FILETIME ftLastWriteTime;      // last write time

    hKeyToOpen = GetHkeyrootFromString(ParamIn1);

#ifdef _WIN64 /* Scilab x64 on x64 windows */
    OpensKeyOptions = KEY_READ  | KEY_WOW64_64KEY;
    if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &hTestKey) != ERROR_SUCCESS )
    {
        OpensKeyOptions = KEY_READ  | KEY_WOW64_32KEY;
        if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &hTestKey) != ERROR_SUCCESS ) return FALSE;
    }
#else
    if (IsWow64()) /* Scilab 32 bits on x64 windows */
    {
        OpensKeyOptions = KEY_READ  | KEY_WOW64_64KEY;
        if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &hTestKey) != ERROR_SUCCESS )
        {
            OpensKeyOptions = KEY_READ  | KEY_WOW64_32KEY;
            if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &hTestKey) != ERROR_SUCCESS ) return FALSE;
        }
    }
    else /* Scilab 32 bits on windows 32 bits */
    {
        OpensKeyOptions = KEY_READ ;
        if ( RegOpenKeyEx(hKeyToOpen, ParamIn2, 0, OpensKeyOptions, &hTestKey) != ERROR_SUCCESS ) return FALSE;
    }
#endif
    retCode = RegQueryInfoKey(
        hTestKey,                // key handle
        achClass,                // buffer for class name
        &cchClassName,           // size of class string
        NULL,                    // reserved
        &cSubKeys,               // number of subkeys
        &cbMaxSubKey,            // longest subkey size
        &cchMaxClass,            // longest class string
        &cValues,                // number of values for this key
        &cchMaxValue,            // longest value name
        &cbMaxValueData,         // longest value data
        &cbSecurityDescriptor,   // security descriptor
        &ftLastWriteTime);       // last write time

    if (retCode != ERROR_SUCCESS)
    {
        bOK = FALSE;
    }
    else
    {
        *Number = cValues;
    }

    RegCloseKey(hKeyToOpen);

    return bOK;
}
/*--------------------------------------------------------------------------*/
