/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include "getVideoAdapters.h"
#include "sci_malloc.h"
#include "BOOL.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
static int getNumberOfAdapters(BOOL *multiDriver);
static void RemoveDuplicateStrings(char **Strings, int *SizeStrings);
/*--------------------------------------------------------------------------*/
char **getVideoAdapters(int *returnedNbAdapters)
{
    BOOL bMultiDriver = FALSE;
    DISPLAY_DEVICE displayDevice;
    DEVMODE devMode;
    char **StringNames = NULL;
    DISPLAY_DEVICE dd;
    int nbAdapters = getNumberOfAdapters(&bMultiDriver);
    memset(&dd, 0, sizeof(DISPLAY_DEVICE));
    *returnedNbAdapters = 0;

    if (nbAdapters > 0)
    {
        StringNames = (char**)MALLOC(sizeof(char*) * nbAdapters);
        if (StringNames)
        {
            DWORD deviceNum = 0;
            DWORD i = 0;
            memset(&displayDevice, 0, sizeof(displayDevice));
            displayDevice.cb = sizeof(DISPLAY_DEVICE);
            while (EnumDisplayDevices(NULL, i, &displayDevice, 0))
            {
                if (displayDevice.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
                {
                }
                else
                {
                    EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode);
                    StringNames[(int)deviceNum] = os_strdup(displayDevice.DeviceString);
                    ++deviceNum;
                }
                ++i;
            }
            *returnedNbAdapters = (int)deviceNum;
            RemoveDuplicateStrings(StringNames, returnedNbAdapters);
        }
    }
    return StringNames;
}
/*--------------------------------------------------------------------------*/
static int getNumberOfAdapters(BOOL *multiDriver)
{
    DWORD deviceNum = 0;
    DWORD i = 0;
    DISPLAY_DEVICE displayDevice;

    memset(&displayDevice, 0, sizeof(displayDevice));
    displayDevice.cb = sizeof(DISPLAY_DEVICE);
    while (EnumDisplayDevices(NULL, i, &displayDevice, 0))
    {
        if (displayDevice.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
        {
        }
        else
        {
            if (displayDevice.StateFlags & DISPLAY_DEVICE_MULTI_DRIVER )
            {
                *multiDriver = TRUE;
            }
            ++deviceNum;
        }
        ++i;
    }
    return deviceNum;
}
/*--------------------------------------------------------------------------*/
static void RemoveDuplicateStrings(char **Strings, int *SizeStrings)
{
    int fin, i;
    int newsize = *SizeStrings;
    for (fin = *SizeStrings - 1; fin > 0; fin--)
    {
        int Sorted = FALSE;
        for (i = 0; i < fin; i++)
        {
            if (Strings[i])
            {
                if (strcmp(Strings[i], Strings[i + 1]) == 0)
                {
                    FREE(Strings[i + 1]);
                    Strings[i + 1] = NULL;
                    Sorted = TRUE;
                    newsize--;
                }
            }
            else
            {
                Strings[i] = Strings[i + 1];
                Strings[i + 1] = NULL;
                Sorted = TRUE;
            }
        }
        if (!Sorted)
        {
            break;
        }
    }

    *SizeStrings = newsize;
}
/*--------------------------------------------------------------------------*/
char * GetPrimaryVideoCardVersion(void)
{
#define KeyDisplayIdentifer "SYSTEM\\ControlSet001\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000"
#define KeyDisplayIdentiferOthers "SYSTEM\\ControlSet002\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000"
#define LenLine 255

    HKEY key;
    DWORD result;
    char *LineIdentifier;
    ULONG length = LenLine, Type;

    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, KeyDisplayIdentifer, 0, KEY_QUERY_VALUE , &key);
    if (result !=  ERROR_SUCCESS)
    {
        // On some configuration (x64 + non official drivers), ControlSet001 does not exist
        result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, KeyDisplayIdentiferOthers, 0, KEY_QUERY_VALUE , &key);
    }

    LineIdentifier = (char*)MALLOC(sizeof(char) * length);

    if ( RegQueryValueEx(key, "DriverVersion", 0, &Type, (LPBYTE)LineIdentifier, &length) !=  ERROR_SUCCESS )
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
