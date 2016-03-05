/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#include "promptecho.h"
#include "basout.h"
#include "sci_malloc.h"
#include "prompt.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
int C2F(promptecho)(int *lunit, char *string, int *strl, int string_len)
{
    char *promptstr = NULL;
    char *buffstr =  NULL;
    int lenbuffstr = 0;

    if (GetTemporaryPrompt())
    {
        promptstr = os_strdup(GetTemporaryPrompt());
    }
    else
    {
        promptstr = (char*)MALLOC(sizeof(char) * (PROMPT_SIZE_MAX + 1));
        GetCurrentPrompt(promptstr);
    }

    /* concates prompt with input command line */
    if (promptstr)
    {
        lenbuffstr = (int) ( strlen(string) + strlen(promptstr) );
        buffstr = (char*) MALLOC(sizeof(char) * (lenbuffstr + 1));
        if (buffstr)
        {
            strcpy(buffstr, promptstr);
            strcat(buffstr, string);
        }
        FREE(promptstr);
        promptstr = NULL;
    }

    /* displays command line with prompt */
    if (buffstr)
    {
        int io = 0;
        C2F(basout)(&io, lunit, buffstr, lenbuffstr);
        FREE(buffstr);
        buffstr = NULL;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
