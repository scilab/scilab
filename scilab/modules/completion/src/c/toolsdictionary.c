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
#include <stdlib.h>
#include <string.h>
#include "toolsdictionary.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
/**
* merge two strings array
* append array2 to array1
* @param[in] array1
* @param[in] pos
* @param[in] array2
* @param[in] sizearray2
* @return string array updated;
*/
static char **mergearrays(char **array1, int pos, char **array2, int sizearray2);

/* compare function for qsort */
static int comparewords(const void *a, const void *b);

/*--------------------------------------------------------------------------*/
BOOL appendDictionary(char ***dictionary, int *i, char ***data, int *sizedata)
{
    BOOL bOK = FALSE;

    if (*data)
    {
        *dictionary = mergearrays(*dictionary, *i, *data, *sizedata);
        *i = *i + *sizedata;

        FREE(*data);
        *data = NULL;
        *sizedata = 0;
        bOK = TRUE;
    }
    return bOK;
}

/*--------------------------------------------------------------------------*/
static char **mergearrays(char **array1, int pos, char **array2, int sizearray2)
{
    if (array1 && array2)
    {
        int j = 0;

        for (j = 0; j < sizearray2; j++)
        {
            array1[pos] = array2[j];
            pos++;
        }
        return array1;
    }
    return NULL;
}

/*--------------------------------------------------------------------------*/
static int comparewords(const void *a, const void *b)
{
    return (strcmp(*(char **)a, *(char **)b));
}

/*--------------------------------------------------------------------------*/
char **SortDictionary(char **Strings, int SizeStrings)
{
    qsort(Strings, SizeStrings, sizeof(char *), comparewords);
    return Strings;
}

/*--------------------------------------------------------------------------*/
char **RemoveDuplicateDictionary(char **Strings, int *SizeStrings)
{
    char **returnedArray = NULL;
    int newsize = 0;

    if (Strings)
    {
        int i = 0, j = 0;

        for (i = j = 0; i < *SizeStrings - 1; i++)
        {
            if (strcmp(Strings[i], Strings[i + 1]))
            {
                Strings[j++] = Strings[i];
            }
            else
            {
                FREE(Strings[i]);
                Strings[i] = NULL;
            }
        }
        Strings[j++] = Strings[i];
        newsize = j;

        returnedArray = Strings;
        if (returnedArray)
        {
            *SizeStrings = newsize;
        }
    }
    return returnedArray;
}

/*--------------------------------------------------------------------------*/
