/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h> 
#include "stack-def.h" /* for paus */
#include "texmacs.h"
#include "prompt.h"
#include "readline.h"
#include "api_scilab.h"
#include "sciprint.h"
#include "localization.h"
#include "warningmode.h"
/*--------------------------------------------------------------------------*/
#if 0 /* to debug TeXmacs interface */
#define DATA_BEGIN  ((char) 'B')
#define DATA_END    ((char) 'E')
#else
#define DATA_BEGIN  ((char) 2)
#define DATA_END    ((char) 5)
#endif
#define DATA_ESCAPE  ((char) 27)
#define Pause C2F(recu).paus
/*--------------------------------------------------------------------------*/
static int first=1;
static int texmacs_mode=0;
/*--------------------------------------------------------------------------*/
void settexmacs(void) 
{
    texmacs_mode = 1;
}
/*--------------------------------------------------------------------------*/
int C2F(intexmacs)(void) 
{
    return texmacs_mode;
}
/*--------------------------------------------------------------------------*/
void next_input (void) 
{
    fprintf(stdout, "%cchannel:prompt%c", DATA_BEGIN,DATA_END);
    if (Pause == 0)
    {
        fprintf(stdout, SCIPROMPT);
    }
    else
    {
        fprintf(stdout, SCIPROMPT_INTERRUPT, Pause);
    }

    fprintf(stdout, "%c", DATA_END);
    fflush (stdout);
}
/*--------------------------------------------------------------------------*/
void C2F(texmacsin)(char buffer[], int *buf_size, int *len_line, int *eof, long int dummy1)
{
    #define STDIN_ID 5
    #define TEXMACSLIB "texmacslib"
    int nr = 0, info = 0;
    if (first == 1) 
    {
        if (isNamedVarExist(pvApiCtx, TEXMACSLIB) == 0)
        {
            if (getWarningMode())
            {
                fprintf(stdout, _("Please install texmacs ATOMS module: atomsInstall('texmacs')\n\n"), _("Warning"));
            }

        }
        fprintf(stdout, "%cverbatim:", DATA_BEGIN);
    }
    next_input ();

    first = 0;
    *eof = 0;
    
    info = LineRead(STDIN_ID, buffer, *buf_size, len_line, &nr);
    fprintf(stdout, "%cverbatim:", DATA_BEGIN);
    *len_line = *len_line - 1;
}
/*--------------------------------------------------------------------------*/
