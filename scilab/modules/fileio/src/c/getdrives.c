/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "getdrives.h"
#include "MALLOC.h"
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
/*--------------------------------------------------------------------------*/
