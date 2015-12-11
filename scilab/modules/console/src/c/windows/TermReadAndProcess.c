/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
