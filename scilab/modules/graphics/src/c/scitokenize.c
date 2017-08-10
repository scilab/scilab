/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
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
#include <string.h>
#include "scitokenize.h"
#include "sci_malloc.h" /* MALLOC */
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int scitokenize(char * legend, char *** Strptr, int *n)
{
    int arobaseIndex = 0;
    int i = 0;
    int count = 1;
    int prevIndex = 0;
    int stringLength = 0, j = 0;
    char **Str = NULL;
    int lenlegend = (int)strlen(legend);

    /* first get the number of tokens */
    for (i = 0; i < lenlegend; i++)
    {
        if (legend[i] == '@')
        {
            count++;
        }
    }
    *Strptr = (char **)MALLOC(count * sizeof(char *));
    Str = *Strptr;
    if (Str == NULL)
    {
        return 1;    /* not enough memory */
    }
    for (i = 0; i < count; i++)
    {
        Str[i] = NULL;
    }

    for (i = 0; i < count; i++)
    {
        while (legend[arobaseIndex] != '@' && legend[arobaseIndex] != 0)
        {
            arobaseIndex++;
        }
        /* we found a token */
        stringLength =  arobaseIndex - prevIndex;
        Str[i] = (char *)MALLOC((stringLength + 1) * sizeof(char)); /*  +1 for null terminating character*/

        if (Str[i] == NULL)
        {
            freeArrayOfString(Str, i - 1);
            return 1;
        }

        for (j = 0; j < stringLength; j++)
        {
            Str[i][j] = legend[prevIndex + j];
        }
        Str[i][stringLength] = 0;

        /* skip the @*/
        arobaseIndex++;
        prevIndex = arobaseIndex;
    }
    *n = count;
    return 0;
}
/*--------------------------------------------------------------------------*/
