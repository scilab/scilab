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
#include <stdio.h>
#include <windows.h>
#include "InitializeWindows_tools.h"
#include "fromc.h"
/*--------------------------------------------------------------------------*/
BOOL InitializeWindows_tools(void)
{
    BOOL bOK = FALSE;

    HWND hScilab = GetConsoleWindow();

    if ( (hScilab) && IsFromC() )
    {
        /* force redirect stdout, stderr in console */
        freopen("CONOUT$", "wb", stdout); /* redirect stdout --> CONOUT$*/
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
