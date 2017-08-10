/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2008 - Allan CORNET
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
#include <windows.h>
#include "sci_malloc.h"
#include "MutexClosingScilab.h"
#include "version.h"
/*--------------------------------------------------------------------------*/
static HANDLE hMutexClosingScilabID = NULL;
/*--------------------------------------------------------------------------*/
static char *getClosingScilabMutexName(void);
/*--------------------------------------------------------------------------*/
void createMutexClosingScilab(void)
{
    char *mutexname = getClosingScilabMutexName();
    if (mutexname)
    {
        HANDLE hMutex = NULL;
        hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexname);
        /* checks if a previous Mutex exists */
        if (!hMutex)
        {
            hMutexClosingScilabID = CreateMutex (NULL, FALSE, mutexname);
        }
        FREE(mutexname);
        mutexname = NULL;
    }
}
/*--------------------------------------------------------------------------*/
void terminateMutexClosingScilab(void)
{
    /* close named mutex */
    if (hMutexClosingScilabID)
    {
        CloseHandle(hMutexClosingScilabID);
    }
}
/*--------------------------------------------------------------------------*/
BOOL haveMutexClosingScilab(void)
{
    char *mutexname = getClosingScilabMutexName();
    if (mutexname)
    {
        HANDLE hMutex;
        hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexname);
        FREE(mutexname);
        mutexname = NULL;
        if (hMutex)
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
static char *getClosingScilabMutexName(void)
{
    int lenmutexname = (int)(strlen(CLOSING_SCILAB_MUTEX_NAME) + strlen(SCI_VERSION_STRING) + 1);
    char *mutexname = (char*)MALLOC(sizeof(char) * lenmutexname);
    if (mutexname)
    {
        strcpy(mutexname, CLOSING_SCILAB_MUTEX_NAME);
        strcat(mutexname, SCI_VERSION_STRING);
    }
    return mutexname;
}
/*--------------------------------------------------------------------------*/
