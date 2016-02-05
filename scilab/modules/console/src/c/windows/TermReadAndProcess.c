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
#include "isatty.hxx"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
char * getCmdLine(void)
{
#ifdef _MSC_VER
    /* if file descriptor returned is -2 stdin is not associated with an input stream */
    /* example : echo plot3d | scilex -nw -e */
    if (!isatty(_fileno(stdin)) && (_fileno(stdin) != -2))
#else
    if (!isatty(fileno(stdin)))
#endif
    {
        char buffer[bsiz];
        //read from stdin
        int eof = (fgets(buffer, bsiz, stdin) == NULL);
        if (eof)
        {
            //send command to quit to Scilab
            return os_strdup("quit");
        }

        //remove trailing \n
        int len_line = (int)strlen(buffer);
        if (buffer[len_line - 1] == '\n')
        {
            buffer[len_line - 1] = '\0';
        }

        return os_strdup(buffer);
    }
    else
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
}
/*--------------------------------------------------------------------------*/
