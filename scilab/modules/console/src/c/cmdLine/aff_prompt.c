/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#include <wchar.h>
#include <wctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <curses.h>
#include "reader.h"
#include "aff_prompt.h"
#include "prompt.h"

/*
 * If there's a string, the function save it.
 * The function return the current saved prompt.
 */

wchar_t *setPrompt(wchar_t * wcs)
{
    static wchar_t *prompt = NULL;

    if (wcs != NULL)
    {
        return wcs;
    }
    return prompt;
}

/*
 * The function get the current prompt
 * If none are set, function set it to the basic prompt SCIPROMPT ("-->") using wchar.
 * If Argument pass is WRT_PRT (1), it write prompt (NOWRT_PRT (-1) not to write prompt)
 * Function return size of the prompt.
 */

int getPrompt(int token)
{
    wchar_t *prompt;

    prompt = setPrompt(NULL);
    if (prompt == NULL)
    {
        prompt = setPrompt((wchar_t *) WSCIPROMPT);
    }
    if (token == WRT_PRT)
    {
        printf("%ls", prompt);
        fflush(stdout);
    }
    return wcslen(prompt);
}
