/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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


/* !!! PLEASE DO NOT TRANSLATE STRINGS IN THIS FILE SEE BUG 5505 !!! */


#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sci_malloc.h"
#include "getDynamicDebugInfo_Windows.h"
#include "localization.h"
#include "getos.h"
#include "api_scilab.h"
#include "charEncoding.h"
#include "os_string.h"
#include "getVideoAdapters.h"
#include "freeArrayOfString.h"
#include "GetWindowsVersion.h"
/*--------------------------------------------------------------------------*/
static char * GetRegKeyCPUIdentifier(void);
static char * GetScreenResolution(void);
static char * GetNumberMonitors(void);
static char ** appendStringDebugInfo(char **listInfo, int *sizeListInfo, char *str);
/*--------------------------------------------------------------------------*/
char **getDynamicDebugInfo_Windows(int *sizeArray)
{
#define DIV 1024
#define WIDTH 7
#define BUFFER_LEN 255

    int nb_info = 0;
    char *str_info = NULL;
    char **outputDynamicList = NULL;
    char *fromGetenv = NULL;
    int iType = 0;

    char **DevicesName = NULL;
    int nbDevices = 0;


    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx (&statex);


    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info,
                "Memory in use: %*ld %%",
                WIDTH,
                statex.dwMemoryLoad);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info,
                "Total Physical Memory (Kbytes): %*I64d",
                WIDTH,
                statex.ullTotalPhys / DIV);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info,
                "Free Physical Memory (Kbytes): %*I64d",
                WIDTH,
                statex.ullAvailPhys / DIV);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info,
                "Total Paging File (Kbytes): %*I64d",
                WIDTH,
                statex.ullTotalPageFile / DIV);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info,
                "Free Paging File (Kbytes): %*I64d",
                WIDTH,
                statex.ullAvailPageFile / DIV);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info,
                "Total Virtual Memory (Kbytes): %*I64d",
                WIDTH,
                statex.ullTotalVirtual / DIV);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info,
                "Free Virtual Memory (Kbytes): %*I64d",
                WIDTH,
                statex.ullAvailVirtual / DIV);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info,
                "Free Extended Memory (Kbytes): %*I64d",
                WIDTH,
                statex.ullAvailExtendedVirtual / DIV);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        char *OS = getOSFullName();
        char *OSRelease = getOSRelease();
        strcpy(str_info , "Operating System: ");

        if (OS && OSRelease)
        {
            strcat(str_info , OS);
            strcat(str_info , " ");
            strcat(str_info , OSRelease);
        }
        else
        {
            strcat(str_info, "ERROR");
        }

        if (OS)
        {
            FREE(OS);
            OS = NULL;
        }
        if (OSRelease)
        {
            FREE(OSRelease);
            OSRelease = NULL;
        }

        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, GetRegKeyCPUIdentifier());

    str_info = (char*)MALLOC( sizeof(char) * BUFFER_LEN );
    if (str_info)
    {
        SYSTEM_INFO siSysInfo;
        GetSystemInfo(&siSysInfo);
        sprintf(str_info, "Number of processors: %d",  siSysInfo.dwNumberOfProcessors);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    DevicesName = getVideoAdapters(&nbDevices);
    if (nbDevices > 0)
    {
        str_info = (char*)MALLOC( sizeof(char) * BUFFER_LEN );
        if (str_info)
        {
            sprintf(str_info, "Number of Video cards: %d", nbDevices);
            outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
        }
    }

    if (nbDevices > 0 && (DevicesName != NULL))
    {
        int i = 0;
        for (i = 0; i < nbDevices; i++)
        {
            str_info = (char*)MALLOC( sizeof(char) * BUFFER_LEN );
            if (str_info)
            {
                sprintf(str_info, "Video card #%d: %s",  i, DevicesName[i]);
                outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
            }
        }
        freeArrayOfString(DevicesName, nbDevices);
        DevicesName = NULL;
    }
    else
    {
        str_info = (char*)MALLOC( sizeof(char) * BUFFER_LEN );
        if (str_info)
        {
            sprintf(str_info, "Video card: %s",  "ERROR");
            outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
        }
    }

    str_info = (char*)MALLOC( sizeof(char) * BUFFER_LEN );
    if (str_info)
    {
        sprintf(str_info, "Primary Video card driver version: %s", GetPrimaryVideoCardVersion());
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, GetScreenResolution());
    outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, GetNumberMonitors());

#define PATH_var "Path"
    fromGetenv = getenv(PATH_var);

    if (fromGetenv)
    {
        str_info = (char*)MALLOC( sizeof(char) * (strlen(fromGetenv) + strlen("%s : %s") + strlen(PATH_var) + 1) );
        sprintf(str_info, "%s: %s", PATH_var, fromGetenv);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

#define COMSPEC_var "ComSpec"
    fromGetenv = getenv(COMSPEC_var);
    if (fromGetenv)
    {
        str_info = (char*)MALLOC( sizeof(char) * (strlen(fromGetenv) + strlen("%s : %s") + strlen(COMSPEC_var) + 1) );
        sprintf(str_info, "%s: %s", COMSPEC_var, fromGetenv);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

#define TMP_var "TMP"
    fromGetenv = getenv(TMP_var);
    if (fromGetenv)
    {
        str_info = (char*)MALLOC( sizeof(char) * (strlen(fromGetenv) + strlen("%s : %s") + strlen(TMP_var) + 1) );
        sprintf(str_info, "%s : %s", TMP_var, fromGetenv);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

#define TEMP_var "TEMP"
    fromGetenv = getenv(TEMP_var);
    if (fromGetenv)
    {
        str_info = (char*)MALLOC( sizeof(char) * (strlen(fromGetenv) + strlen("%s : %s") + strlen(TEMP_var) + 1) );
        sprintf(str_info, "%s: %s", TEMP_var, fromGetenv);
        outputDynamicList = appendStringDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    /*
    sciErr = getNamedVarType(pvApiCtx, "TMPDIR", &iType);
    if ((sciErr.iErr == 0) && (iType == sci_strings))
    {
        wchar_t * TMPDIR_value = NULL;
        int TMPDIR_length = 0;
        int m = 0, n = 0;

        sciErr = readNamedMatrixOfWideString(pvApiCtx, "TMPDIR", &m, &n, &TMPDIR_length, &TMPDIR_value);
        if ( (sciErr.iErr == 0) && ((m == 1) && (n == 1)) )
        {
            TMPDIR_value = (wchar_t*)MALLOC(sizeof(wchar_t)*(TMPDIR_length + 1));
            if (TMPDIR_value)
            {
                sciErr = readNamedMatrixOfWideString(pvApiCtx, "TMPDIR", &m, &n, &TMPDIR_length, &TMPDIR_value);
                if(sciErr.iErr == 0)
                {
                    char *utfstr = wide_string_to_UTF8(TMPDIR_value);
                    if (utfstr)
                    {
                        str_info = (char*)MALLOC( sizeof(char)*(strlen("TMPDIR") + strlen("%s : %s") + strlen(utfstr) + 1) );
                        sprintf(str_info,"%s: %s", "TMPDIR", utfstr);
                        outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
                        FREE(utfstr);
                        utfstr = NULL;
                    }
                }
                FREE(TMPDIR_value);
                TMPDIR_value = NULL;
            }
        }
    }

    sciErr = getNamedVarType(pvApiCtx, "WSCI", &iType);
    if ((sciErr.iErr == 0) && (iType == sci_strings))
    {
        wchar_t * WSCI_value = NULL;
        int WSCI_length = 0;
        int m = 0, n = 0;

        sciErr = readNamedMatrixOfWideString(pvApiCtx, "WSCI", &m, &n, &WSCI_length, &WSCI_value);
        if ( (sciErr.iErr == 0) && ((m == 1) && (n == 1)) )
        {
            WSCI_value = (wchar_t*)MALLOC(sizeof(wchar_t)*(WSCI_length + 1));
            if (WSCI_value)
            {
                sciErr = readNamedMatrixOfWideString(pvApiCtx, "WSCI", &m, &n, &WSCI_length, &WSCI_value);
                if(sciErr.iErr == 0)
                {
                    char *utfstr = wide_string_to_UTF8(WSCI_value);
                    if (utfstr)
                    {
                        str_info = (char*)MALLOC( sizeof(char)*(strlen("WSCI") + strlen("%s : %s") + strlen(utfstr) + 1) );
                        sprintf(str_info,"%s: %s", "WSCI", utfstr);
                        outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
                        FREE(utfstr);
                        utfstr = NULL;
                    }
                }
                FREE(WSCI_value);
                WSCI_value = NULL;
            }
        }
    }

    sciErr = getNamedVarType(pvApiCtx, "SCIHOME", &iType);
    if ((sciErr.iErr == 0) && (iType == sci_strings))
    {
        wchar_t * SCIHOME_value = NULL;
        int SCIHOME_length = 0;
        int m = 0, n = 0;

        sciErr = readNamedMatrixOfWideString(pvApiCtx, "SCIHOME", &m, &n, &SCIHOME_length, &SCIHOME_value);
        if ( (sciErr.iErr == 0) && ((m == 1) && (n == 1)) )
        {
            SCIHOME_value = (wchar_t*)MALLOC(sizeof(wchar_t)*(SCIHOME_length + 1));
            if (SCIHOME_value)
            {
                sciErr = readNamedMatrixOfWideString(pvApiCtx, "SCIHOME", &m, &n, &SCIHOME_length, &SCIHOME_value);
                if(sciErr.iErr == 0)
                {
                    char *utfstr = wide_string_to_UTF8(SCIHOME_value);
                    if (utfstr)
                    {
                        str_info = (char*)MALLOC( sizeof(char)*(strlen("SCIHOME") + strlen("%s : %s") + strlen(utfstr) + 1) );
                        sprintf(str_info,"%s: %s", "SCIHOME", utfstr);
                        outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
                        FREE(utfstr);
                        utfstr = NULL;
                    }
                }
                FREE(SCIHOME_value);
                SCIHOME_value = NULL;
            }
        }
    }
    */
    *sizeArray = nb_info;
    return outputDynamicList;
}
/*--------------------------------------------------------------------------*/
static char * GetRegKeyCPUIdentifier(void)
{
#define KeyCpuIdentifer "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"
#define LenLine 255

    HKEY key;
    DWORD result;
    char *LineIdentifier;
    ULONG length = LenLine, Type;

    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, KeyCpuIdentifer, 0, KEY_QUERY_VALUE , &key);

    LineIdentifier = (char*)MALLOC(sizeof(char) * length);

    if ( RegQueryValueEx(key, "ProcessorNameString", 0, &Type, (LPBYTE)LineIdentifier, &length) !=  ERROR_SUCCESS )
    {
        wsprintf(LineIdentifier, "ERROR");
    }

    if ( Type != REG_SZ )
    {
        wsprintf(LineIdentifier, "ERROR");
    }

    if ( result == ERROR_SUCCESS )
    {
        RegCloseKey(key);
    }

    return (char *)LineIdentifier;
}
/*--------------------------------------------------------------------------*/
static char * GetScreenResolution(void)
{
    HDC hdc = GetDC(NULL);
    int BitsByPixel = GetDeviceCaps(hdc, BITSPIXEL);
    int ResX = GetSystemMetrics(SM_CXSCREEN);
    int ResY = GetSystemMetrics(SM_CYSCREEN) ;
    char *Resolution = NULL;

    ReleaseDC (NULL, hdc);

    Resolution = (char*)MALLOC( sizeof(char) * ( strlen("Screen size: %d x %d %d bits") + 32));
    if (Resolution)
    {
        sprintf(Resolution, "Screen size: %d x %d %d bits", ResX , ResY, BitsByPixel);
    }

    return Resolution;
}
/*--------------------------------------------------------------------------*/
static char ** appendStringDebugInfo(char **listInfo, int *sizeListInfo, char *str)
{
    char ** returnListInfo = NULL;
    if (listInfo)
    {
        (*sizeListInfo)++;
        returnListInfo = (char**)REALLOC(listInfo, sizeof(char*) * (*sizeListInfo));
    }
    else
    {
        (*sizeListInfo) = 1;
        returnListInfo = (char**)MALLOC(sizeof(char*) * (*sizeListInfo));
    }
    returnListInfo[(*sizeListInfo) - 1] = str;

    return returnListInfo;
}
/*--------------------------------------------------------------------------*/
static char * GetNumberMonitors(void)
{
#define NBMONITORS "Number of Monitors: %d"
    char *returnedStr = NULL;
    int nbMonitors = GetSystemMetrics(SM_CMONITORS) ;

    returnedStr = (char*)MALLOC( sizeof(char) * ( strlen(NBMONITORS) +  32));
    if (returnedStr)
    {
        sprintf(returnedStr, NBMONITORS, nbMonitors);
    }

    return returnedStr;
}
/*--------------------------------------------------------------------------*/
