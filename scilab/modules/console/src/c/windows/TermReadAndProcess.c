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
#include "TermReadAndProcess.h"
#include "sci_malloc.h"
#include "prompt.h"
#include "TermConsole.h"
/*--------------------------------------------------------------------------*/
char * getCmdLine(void)
{
    char *line = NULL;
    static char save_prompt[PROMPT_SIZE_MAX];

    if (GetTemporaryPrompt() != NULL) /* Input function is used */
    {
        line = TerminalGetString(GetTemporaryPrompt());
    }
    else
    {
        GetCurrentPrompt(save_prompt);
        line = TerminalGetString(save_prompt);
    }
    strcpy(save_prompt, "");
    return line;
}
/*--------------------------------------------------------------------------*/
