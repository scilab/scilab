/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - 2008 - Allan CORNET
* Copyright (C) DIGITEO - 2012 - Allan CORNET
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
#include "InnosetupMutex.h"
#include "BOOL.h"
#include "version.h"
/*--------------------------------------------------------------------------*/
static HANDLE hMutexScilabID;
/*--------------------------------------------------------------------------*/
void createInnosetupMutex(void)
{
    /* http://www.vincenzo.net/isxkb/index.php?title=Application_considerations */
    /* creates a named mutex used by Innosetup */
    hMutexScilabID = CreateMutex (NULL, FALSE, SCI_VERSION_STRING );
}
/*--------------------------------------------------------------------------*/
void closeInnosetupMutex(void)
{
    /* close named mutex */
    CloseHandle(hMutexScilabID);
}
/*--------------------------------------------------------------------------*/
BOOL haveInnosetupMutex(void)
{
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, SCI_VERSION_STRING);
    if (hMutex)
    {
        CloseHandle(hMutex);
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
