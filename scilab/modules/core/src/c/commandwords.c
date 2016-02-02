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
#include <string.h>
#include "commandwords.h"
#include "sci_malloc.h"
#include "BOOL.h"
#include "os_string.h"


/*--------------------------------------------------------------------------*/
/* if you update command.f , please update this file too ... */
/*--------------------------------------------------------------------------*/
static const int nbrCommands = 29;
/*--------------------------------------------------------------------------*/
static char *CommandWords[] =
{
    "if", "else",
    "for", "while",
    "end", "select",
    "case", "quit",
    "return", "help",
    "what", "who",
    "pause", "clear",
    "resume", "then",
    "do", "apropos",
    "abort", "break",
    "elseif", "pwd",
    "function", "endfunction",
    "clc", "continue",
    "try", "catch",
    "exit"
};
/*--------------------------------------------------------------------------*/
static void SortStrings(char **Strs, int SizeOfStrs);
/*--------------------------------------------------------------------------*/
char **getcommandkeywords(int *sizearray)
{
    char **keywords = NULL;

    keywords = (char**)MALLOC(sizeof(char*) * nbrCommands);

    if (keywords)
    {
        int i = 0;
        for (i = 0; i < nbrCommands; i++)
        {
            keywords[i] = os_strdup(CommandWords[i]);
        }
        *sizearray =  nbrCommands;

        SortStrings(keywords, *sizearray);
    }
    else
    {
        *sizearray = 0;
    }

    return keywords;
}
/*--------------------------------------------------------------------------*/
static void SortStrings(char **Strs, int SizeOfStrs)
{
    int fin, i;
    for (fin = SizeOfStrs - 1; fin > 0; fin--)
    {
        int Sorted = FALSE;
        for (i = 0; i < fin; i++)
        {
            if (strcmp(Strs[i], Strs[i + 1]) > 0)
            {
                char *tmp;
                memcpy(&tmp, Strs + i, sizeof(char *));
                memcpy(Strs + i, Strs + (i + 1), sizeof(char *));
                memcpy(Strs + (i + 1), &tmp, sizeof(char *));
                Sorted = TRUE;
            }
        }
        if (!Sorted)
        {
            break;
        }
    }
}
/*--------------------------------------------------------------------------*/
