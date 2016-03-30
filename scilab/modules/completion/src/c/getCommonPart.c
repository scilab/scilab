/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
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
#include <stdlib.h>
#include "getCommonPart.h"
#include "core_math.h"
#include "sci_malloc.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
static int cmp( const void *a , const void *b)
{
    return strcmp(*(char**)a, *(char**)b );
}
/*--------------------------------------------------------------------------*/
static int cmpPos(char *str1, char *str2)
{
    if (str1 && str2)
    {
        int i = 0;
        char *s1 = NULL;
        char *s2 = NULL;
        int lenstr1 = (int) strlen(str1);
        int lenstr2 = (int) strlen(str2);
        int slen;

        if (lenstr1 > lenstr2)
        {
            s1 = str2;
            s2 = str1;
            slen = lenstr1;
            lenstr1 = lenstr2;
            lenstr2 = slen;
        }
        else
        {
            s1 = str1;
            s2 = str2;
        }
        for (i = 0; i < lenstr1; i++)
        {
            if ( s1[i] != s2[i] )
            {
                return i;
            }
        }

        return Min(lenstr1, lenstr2);
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
char *getCommonPart(char **dictionary, int sizeDictionary)
{
    char *commonpart = NULL;

    if (sizeDictionary == 1)
    {
        return os_strdup(dictionary[0]);
    }

    if (sizeDictionary >= 2)
    {
        int i = 0;
        int r = 0;
        char *currentstr = NULL;
        qsort(dictionary, sizeDictionary, sizeof dictionary[0], cmp);
        currentstr = dictionary[0];

        r = cmpPos(currentstr, dictionary[1]);
        for (i = 1; i < sizeDictionary - 1; i++)
        {
            int current_r = cmpPos(currentstr, dictionary[i + 1]);
            if (r > current_r)
            {
                r = current_r;
                currentstr = dictionary[i + 1];
            }
        }

        commonpart = os_strdup(currentstr);
        commonpart[r] = '\0';
    }
    return commonpart;
}
/*--------------------------------------------------------------------------*/
