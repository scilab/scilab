/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include <stdio.h>
#include "getCpuInfos.h"
/*--------------------------------------------------------------------------*/
#define KeyCentralProcessor L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"
#define KeyVendorIdentifier L"VendorIdentifier"
#define KeyProcessorNameString L"ProcessorNameString"
#define KeyPlatformID L"Platform ID"
#define KeyCpuIdentifier L"Identifier"
#define KeyFrequency L"~Mhz"
#define LenMaxLine MAX_PATH * 2
/*--------------------------------------------------------------------------*/
wchar_t *getCpuVendor(void)
{
    wchar_t *CpuManufacturer = NULL;
    HKEY key;
    DWORD result;
    ULONG length = LenMaxLine;
    ULONG Type = 0;

    result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, KeyCentralProcessor, 0, KEY_QUERY_VALUE , &key);
    if (result == ERROR_SUCCESS)
    {
        wchar_t LineResult[LenMaxLine];

        if ( RegQueryValueExW(key, KeyVendorIdentifier,
                              0, &Type, (LPBYTE)LineResult, &length) ==  ERROR_SUCCESS )
        {
            if ( Type == REG_SZ )
            {
                CpuManufacturer = _wcsdup(LineResult);
            }
        }
        RegCloseKey(key);
    }
    return CpuManufacturer;
}
/*--------------------------------------------------------------------------*/
wchar_t *getProcessorNameString(void)
{
    wchar_t *ProcessorNameString = NULL;
    HKEY key;
    DWORD result;
    ULONG length = LenMaxLine;
    ULONG Type = 0;

    result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, KeyCentralProcessor, 0, KEY_QUERY_VALUE , &key);
    if (result == ERROR_SUCCESS)
    {
        wchar_t LineResult[LenMaxLine];

        if ( RegQueryValueExW(key, KeyProcessorNameString,
                              0, &Type, (LPBYTE)LineResult, &length) ==  ERROR_SUCCESS )
        {
            if ( Type == REG_SZ )
            {
                ProcessorNameString = _wcsdup(LineResult);
            }
        }
        RegCloseKey(key);
    }
    return ProcessorNameString;
}
/*--------------------------------------------------------------------------*/
int getCpuPlatformID(void)
{
    int CpuPlatformId = CPU_PLATFORMID_ERROR;
    HKEY key;
    DWORD result;

    result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, KeyCentralProcessor, 0, KEY_QUERY_VALUE , &key);
    if (result == ERROR_SUCCESS)
    {
        ULONG type = 0;
        DWORD size = 4;
        int Num = 0;
        if (RegQueryValueExW(key, KeyPlatformID, NULL, &type, (LPBYTE)&Num, &size) == ERROR_SUCCESS )
        {
            if (type == REG_DWORD)
            {
                CpuPlatformId = Num;
            }
        }
        RegCloseKey(key);
    }
    return CpuPlatformId;
}
/*--------------------------------------------------------------------------*/
wchar_t *getCpuIdentifier(void)
{
    wchar_t *CpuIdentifier = NULL;
    HKEY key;
    DWORD result;
    ULONG length = LenMaxLine;
    ULONG Type = 0;

    result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, KeyCentralProcessor, 0, KEY_QUERY_VALUE , &key);
    if (result == ERROR_SUCCESS)
    {
        wchar_t LineResult[LenMaxLine];

        if ( RegQueryValueExW(key, KeyCpuIdentifier,
                              0, &Type, (LPBYTE)LineResult, &length) ==  ERROR_SUCCESS )
        {
            if ( Type == REG_SZ )
            {
                CpuIdentifier = _wcsdup(LineResult);
            }
        }
        RegCloseKey(key);
    }
    return CpuIdentifier;
}
/*--------------------------------------------------------------------------*/
int getCpuFrequency(void)
{
    int CpuFrequency = CPU_FREQUENCY_ERROR;
    HKEY key;
    DWORD result;

    result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, KeyCentralProcessor, 0, KEY_QUERY_VALUE , &key);
    if (result == ERROR_SUCCESS)
    {
        ULONG type = 0;
        DWORD size = 4;
        int Num = 0;
        if (RegQueryValueExW(key, KeyFrequency, NULL, &type, (LPBYTE)&Num, &size) == ERROR_SUCCESS )
        {
            if (type == REG_DWORD)
            {
                CpuFrequency = Num;
            }
        }
        RegCloseKey(key);
    }
    return CpuFrequency;
}
/*--------------------------------------------------------------------------*/
int getCpuFamily(void)
{
    wchar_t *Identifier = getCpuIdentifier();
    int CpuFamily = CPU_FAMILY_ERROR;
    if (Identifier)
    {
        wchar_t wArch[LenMaxLine];
        wchar_t wFamily[LenMaxLine];
        wchar_t wFamilyValue[LenMaxLine];
        wchar_t wModel[LenMaxLine];
        wchar_t wModelValue[LenMaxLine];
        wchar_t wStepping[LenMaxLine];
        wchar_t wSteppingValue[LenMaxLine];

        swscanf(Identifier, L"%s %s %s %s %s %s %s",
                wArch,
                wFamily, wFamilyValue,
                wModel, wModelValue,
                wStepping, wSteppingValue);

        CpuFamily = (int)_wcstoi64(wFamilyValue, (wchar_t **)NULL, 10);

        free(Identifier);
        Identifier = NULL;
    }
    return CpuFamily;
}
/*--------------------------------------------------------------------------*/
int getCpuModel(void)
{
    wchar_t *Identifier = getCpuIdentifier();
    int CpuModel = CPU_MODEL_ERROR;
    if (Identifier)
    {
        wchar_t wArch[LenMaxLine];
        wchar_t wFamily[LenMaxLine];
        wchar_t wFamilyValue[LenMaxLine];
        wchar_t wModel[LenMaxLine];
        wchar_t wModelValue[LenMaxLine];
        wchar_t wStepping[LenMaxLine];
        wchar_t wSteppingValue[LenMaxLine];

        swscanf(Identifier, L"%s %s %s %s %s %s %s",
                wArch,
                wFamily, wFamilyValue,
                wModel, wModelValue,
                wStepping, wSteppingValue);

        CpuModel = (int)_wcstoi64(wModelValue, (wchar_t **)NULL, 10);

        free(Identifier);
        Identifier = NULL;
    }
    return CpuModel;
}
/*--------------------------------------------------------------------------*/
int getCpuStepping(void)
{
    wchar_t *Identifier = getCpuIdentifier();
    int CpuStepping = CPU_STEPPING_ERROR;
    if (Identifier)
    {
        wchar_t wArch[LenMaxLine];
        wchar_t wFamily[LenMaxLine];
        wchar_t wFamilyValue[LenMaxLine];
        wchar_t wModel[LenMaxLine];
        wchar_t wModelValue[LenMaxLine];
        wchar_t wStepping[LenMaxLine];
        wchar_t wSteppingValue[LenMaxLine];

        swscanf(Identifier, L"%s %s %s %s %s %s %s",
                wArch,
                wFamily, wFamilyValue,
                wModel, wModelValue,
                wStepping, wSteppingValue);

        CpuStepping = (int)_wcstoi64(wSteppingValue, (wchar_t **)NULL, 10);

        free(Identifier);
        Identifier = NULL;
    }
    return CpuStepping;
}
/*--------------------------------------------------------------------------*/
wchar_t *getCpuArchitecture(void)
{
    wchar_t *CpuArchitecture = NULL;
    wchar_t *Identifier = getCpuIdentifier();

    if (Identifier)
    {
        wchar_t wArch[LenMaxLine];
        wchar_t wFamily[LenMaxLine];
        wchar_t wFamilyValue[LenMaxLine];
        wchar_t wModel[LenMaxLine];
        wchar_t wModelValue[LenMaxLine];
        wchar_t wStepping[LenMaxLine];
        wchar_t wSteppingValue[LenMaxLine];

        swscanf(Identifier, L"%s %s %s %s %s %s %s",
                wArch,
                wFamily, wFamilyValue,
                wModel, wModelValue,
                wStepping, wSteppingValue);

        CpuArchitecture = _wcsdup(wArch);

        free(Identifier);
        Identifier = NULL;
    }
    return CpuArchitecture;
}
/*--------------------------------------------------------------------------*/
