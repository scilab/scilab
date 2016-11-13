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
#include <Windows.h>
#include "winopen.h"
#include "expandPathVariable.h"
#include "Scierror.h"
#include "PATH_MAX.h"
#include "charEncoding.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
BOOL winopen(const  char* scilabfilename)
{
    BOOL bOK = FALSE;
    char *filename = NULL;
    HINSTANCE error = NULL;

    filename = expandPathVariable(scilabfilename);
    if (filename)
    {
        error = ShellExecuteA(NULL, "open", filename, NULL, NULL, SW_SHOWNORMAL);
        if ( error <= (HINSTANCE)32)
        {
            bOK = FALSE;
        }
        else
        {
            bOK = TRUE;
        }

        FREE(filename);
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
