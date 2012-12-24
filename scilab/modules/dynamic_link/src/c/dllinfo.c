/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2011 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*---------------------------------------------------------------------------*/
#include <windows.h>
#include <Dbghelp.h>
#include "dllinfo.h"
/*---------------------------------------------------------------------------*/
#pragma comment(lib,"Dbghelp.lib")
/*---------------------------------------------------------------------------*/
static PIMAGE_NT_HEADERS getPEHeader(const wchar_t *dllfilename);
/*---------------------------------------------------------------------------*/
BOOL isDll(const char *dllfilename)
{
    if (dllfilename)
    {
        wchar_t *wcfilename = to_wide_string(dllfilename);
        if (wcfilename)
        {
            BOOL bRes = isDllW(wcfilename);
            FREE(wcfilename);
            return bRes;
        }
    }
    return FALSE;
}
/*---------------------------------------------------------------------------*/
BOOL isX64Dll(const char *dllfilename)
{
    if (dllfilename)
    {
        wchar_t *wcfilename = to_wide_string(dllfilename);
        if (wcfilename)
        {
            BOOL bRes = isX64DllW(wcfilename);
            FREE(wcfilename);
            return bRes;
        }
    }
    return FALSE;
}
/*---------------------------------------------------------------------------*/
BOOL isX86Dll(const char *dllfilename)
{
    if (dllfilename)
    {
        wchar_t *wcfilename = to_wide_string(dllfilename);
        if (wcfilename)
        {
            BOOL bRes = isX86DllW(wcfilename);
            FREE(wcfilename);
            return bRes;
        }
    }
    return FALSE;
}
/*---------------------------------------------------------------------------*/
BOOL isDllW(const wchar_t *dllfilename)
{
    PIMAGE_NT_HEADERS peHeader = getPEHeader(dllfilename);
    if (peHeader)
    {
        return ((peHeader->FileHeader.Characteristics & IMAGE_FILE_DLL) == IMAGE_FILE_DLL);
    }
    return FALSE;
}
/*---------------------------------------------------------------------------*/
BOOL isX64DllW(const wchar_t *dllfilename)
{
    PIMAGE_NT_HEADERS peHeader = getPEHeader(dllfilename);
    if (peHeader)
    {
        return ((peHeader->FileHeader.Machine & IMAGE_FILE_MACHINE_AMD64) == IMAGE_FILE_MACHINE_AMD64);
    }
    return FALSE;
}
/*---------------------------------------------------------------------------*/
BOOL isX86DllW(const wchar_t *dllfilename)
{
    PIMAGE_NT_HEADERS peHeader = getPEHeader(dllfilename);
    if (peHeader)
    {
        return ((peHeader->FileHeader.Machine & IMAGE_FILE_MACHINE_I386) == IMAGE_FILE_MACHINE_I386);
    }
    return FALSE;
}
/*---------------------------------------------------------------------------*/
static PIMAGE_NT_HEADERS getPEHeader(const wchar_t *dllfilename)
{
    HANDLE hMap = INVALID_HANDLE_VALUE;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    void* mapAddr = NULL;
    PIMAGE_NT_HEADERS peHdr = NULL;


    hFile = CreateFileW(dllfilename, 
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL);

    if (hFile == INVALID_HANDLE_VALUE) return NULL;

    hMap = CreateFileMapping(hFile, 
        NULL,           // security attrs
        PAGE_READONLY,  // protection flags
        0,              // max size - high DWORD
        0,              // max size - low DWORD      
        NULL );         // mapping name - not used

    if (hMap == INVALID_HANDLE_VALUE) 
    {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
        return NULL;
    }

    // next, map the file to our address space
    mapAddr = MapViewOfFileEx( 
        hMap,             // mapping object
        FILE_MAP_READ,  // desired access
        0,              
        0,              
        0,              
        NULL );         // suggested map addr

    if (mapAddr == NULL)
    {
        CloseHandle(hMap);
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
        hMap = INVALID_HANDLE_VALUE;
        return NULL;
    }

    peHdr = ImageNtHeader( mapAddr );
    if (peHdr)
    {
        if (peHdr->FileHeader.SizeOfOptionalHeader == 0)
        {
            peHdr = NULL;
        }
    }

    CloseHandle(hMap);
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
    hMap = INVALID_HANDLE_VALUE;

    return peHdr;
}
/*---------------------------------------------------------------------------*/
