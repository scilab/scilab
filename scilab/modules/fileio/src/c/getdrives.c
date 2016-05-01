/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
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
#include <string.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "getdrives.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
char **getdrives(int *nbDrives)
{
    char **DrivesList = NULL;
    *nbDrives = 0;
#ifdef _MSC_VER
    {
#define DriveMask 0x00000001L
        char DrvLetter[4] = "A:\\";
        DWORD uDriveMask = GetLogicalDrives();

        while (DrvLetter[0] <= 'Z')
        {
            if (uDriveMask & DriveMask)
            {
                (*nbDrives)++;
                if (DrivesList)
                {
                    DrivesList = (char**)REALLOC(DrivesList, sizeof(char*) * (*nbDrives));
                    DrivesList[*nbDrives - 1] = (char*)MALLOC(sizeof(char) * (strlen(DrvLetter) + 1));
                }
                else
                {
                    DrivesList = (char**)MALLOC(sizeof(char*) * (*nbDrives));
                    DrivesList[*nbDrives - 1] = (char*)MALLOC(sizeof(char) * (strlen(DrvLetter) + 1));
                }
                strcpy(DrivesList[*nbDrives - 1], DrvLetter);
            }
            DrvLetter[0]++;
            uDriveMask = uDriveMask >> 1;
        }
    }
#else
    (*nbDrives)++;
    DrivesList = (char**)MALLOC(sizeof(char*) * (*nbDrives));
    DrivesList[*nbDrives - 1] = (char*)MALLOC(sizeof(char) * (strlen("/") + 1));
    strcpy(DrivesList[*nbDrives - 1], "/");
#endif

    return DrivesList;
}

wchar_t **getdrivesW(int *nbDrives)
{
    wchar_t **DrivesList = NULL;
    *nbDrives = 0;
#ifdef _MSC_VER
    {
#define DriveMask 0x00000001L
        wchar_t DrvLetter[4] = L"A:\\";
        DWORD uDriveMask = GetLogicalDrives();

        while (DrvLetter[0] <= 'Z')
        {
            if (uDriveMask & DriveMask)
            {
                (*nbDrives)++;
                if (DrivesList)
                {
                    DrivesList = (wchar_t**)REALLOC(DrivesList, sizeof(wchar_t*) * (*nbDrives));
                    DrivesList[*nbDrives - 1] = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(DrvLetter) + 1));
                }
                else
                {
                    DrivesList = (wchar_t**)MALLOC(sizeof(wchar_t*) * (*nbDrives));
                    DrivesList[*nbDrives - 1] = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(DrvLetter) + 1));
                }
                wcscpy(DrivesList[*nbDrives - 1], DrvLetter);
            }
            DrvLetter[0]++;
            uDriveMask = uDriveMask >> 1;
        }
    }
#else
    (*nbDrives)++;
    DrivesList = (wchar_t**)MALLOC(sizeof(wchar_t*) * (*nbDrives));
    DrivesList[*nbDrives - 1] = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(L"/") + 1));
    wcscpy(DrivesList[*nbDrives - 1], L"/");
#endif

    return DrivesList;
}
/*--------------------------------------------------------------------------*/
