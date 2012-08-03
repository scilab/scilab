/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) Calixte DENIZET - 2010
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <math.h>
#include <string.h>
#include "version.h"
#include "sciprint.h"
#include "banier.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "localization.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
static char *line = "        ___________________________________________        ";
/*--------------------------------------------------------------------------*/
static void centerLine(char *str);
static void centerPrint(char *str);
/*--------------------------------------------------------------------------*/
void banner(void)
{
    sciprint("%s\n", line);

    centerPrint(SCI_VERSION_STRING);
    sciprint("\n\n");

    centerPrint(_("Scilab Enterprises\n"));
    centerPrint(_("Copyright (c) 2011-2012 (Scilab Enterprises)\n"));
    centerPrint(_("Copyright (c) 1989-2012 (INRIA)\n"));
    centerPrint(_("Copyright (c) 1989-2007 (ENPC)\n"));

    sciprint("%s\n", line);
}
/*--------------------------------------------------------------------------*/
static void centerLine(char *str)
{
    int i = 0, start = 0;
    wchar_t *wstr = to_wide_string(str);

    if (wstr)
    {
        char *whites = NULL;
        start = (int)(floor((double)(strlen(line) / 2)) - floor((double)(((int)wcslen(wstr) - 1) / 2)));
        FREE(wstr);
        whites = (char*)MALLOC(sizeof(char) * (start + 1));
        if (whites)
        {
            /* To center the string */
            for ( i = 0 ; i < start ; i++ )
            {
                whites[i] = ' ';
            }
            whites[start] = '\0';
            sciprint(whites);
            FREE(whites);
        }
    }
}
/*--------------------------------------------------------------------------*/
static void centerPrint(char *str)
{
    centerLine(str);
    sciprint(str);
}
/*--------------------------------------------------------------------------*/
int C2F(banier)(int *flag)
{
    if (*flag != 999)
    {
        banner();
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
