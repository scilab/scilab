/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#include "killScilabProcess.h"
/*--------------------------------------------------------------------------*/
void killScilabProcess(int exitCode)
{
    HANDLE hProcess;

    /* Ouverture de ce Process avec droit pour le tuer */
    hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetCurrentProcessId());
    if (hProcess)
    {
        /* Tue ce Process */
        TerminateProcess(hProcess, exitCode);
    }
    else
    {
        MessageBox(NULL, "Don't Find Scilab Process", "Warning", MB_ICONWARNING);
    }
}
/*--------------------------------------------------------------------------*/
