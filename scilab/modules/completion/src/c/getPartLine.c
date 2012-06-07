/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008-2012 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "getPartLine.h"
#include "core_math.h"
/*--------------------------------------------------------------------------*/
char *getPartLevel(char *line)
{
#define MAX_SYMBS 28
    const char symbs[MAX_SYMBS] = "+-*/\\([ ^,;={.&|\'])}:\"\'><~@\t";
    int index = -1;
    int i = 0;

    for (i = 0; i < MAX_SYMBS; i++) 
    {
        int len = 0;
        char *pch = strrchr(line, symbs[i]);
        if (pch) 
        {
            len = (int) (strlen(line) - strlen(pch));
            index = Max(index, len);
        }
    }

    return strdup(&line[index + 1]);
}
/*--------------------------------------------------------------------------*/
char *getFilePartLevel(char *line)
{
#define MAX_SYMBS_F 4
#define SPACE_CHAR ' '
    int index = 0;
    int i = 0;
    int lenLine = 0;
    int symbol_found = 0;
    char symbs[MAX_SYMBS_F] = ";,'\"";

    if (line == NULL) return NULL;
    lenLine = (int)strlen(line);

    /* search last character in ";,'\"" */
    for (i = 0; i < MAX_SYMBS_F; i++) 
    {
        char *prch = strrchr(line, symbs[i]);
        if (prch) 
        {
            int len = (int) (lenLine - strlen(prch));
            index = Max(index, len);
            symbol_found = 1;
            break;
        }
    }

    if (!symbol_found)
    {
        /* search last and first space character */
        char *prch = strrchr(line, SPACE_CHAR);
        char *pch = strchr(line, SPACE_CHAR);
        if (pch && prch)
        {
            int len = 0;
            if (pch != prch)
            {
                len = (int) (strlen(line) - strlen(pch));
            }
            else
            {
                len = (int) (strlen(line) - strlen(prch));
            }
            index = Max(index, len);
            symbol_found = 1;
        }
    }

    if (symbol_found)
    {
        index++;
        /* skip spaces if there are consecutive */
        while (line[index] == SPACE_CHAR)
        {
            if (index + 1 >= lenLine)
            {
                break;
            }
            else
            {
                index++;
            }
        }
        return strdup(&line[index]);
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
