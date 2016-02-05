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
#include "TerminateWindows_tools.h"
#include "configvariable_interface.h"
#include "console.h"
#include "InnosetupMutex.h"
/*--------------------------------------------------------------------------*/
BOOL TerminateWindows_tools(void)
{
    BOOL bOK = FALSE;
    int scilabMode = getScilabMode();

    switch (scilabMode)
    {
        case SCILAB_STD:
        {
            //Since Scilab 6 All scilab mode are launch in console mode
            //So we cannot close owner console
            //CloseScilabConsole();
            closeInnosetupMutex();
            bOK = TRUE;
        }
        break;

        case SCILAB_NW:
        {
            RestoreConsoleColors();
            RestoreExitButton();
            bOK = TRUE;
        }
        break;

        default:
            break;
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
