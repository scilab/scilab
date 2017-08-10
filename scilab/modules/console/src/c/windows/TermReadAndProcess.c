/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <string.h>

#include "prompt.h"
#include "TermReadAndProcess.h"
#include "sci_malloc.h"
#include "TermConsole.h"
/*--------------------------------------------------------------------------*/
char * getCmdLine(void)
{
    const char* tmp = GetTemporaryPrompt();
    if (tmp[0] != 0) /* Input function is used */
    {
        return TerminalGetString(tmp);
    }
    else
    {
        return TerminalGetString(GetCurrentPrompt());
    }
}
/*--------------------------------------------------------------------------*/
