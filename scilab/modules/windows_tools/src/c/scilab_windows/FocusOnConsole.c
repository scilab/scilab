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
#include "dynlib_scilab_windows.h"
#include "configvariable_interface.h"
/*--------------------------------------------------------------------------*/
SCILAB_WINDOWS_IMPEXP void setFocusOnConsole(void)
{
    if ( (getScilabMode() == SCILAB_NW) || (getScilabMode() == SCILAB_NWNI) )
    {
        HWND hWndConsole = GetConsoleWindow();
        if (hWndConsole)
        {
            SetForegroundWindow(hWndConsole);
            SetActiveWindow(hWndConsole);
        }
    }
}
/*--------------------------------------------------------------------------*/
