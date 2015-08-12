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
#include <string.h>
#include "completion_generic.h"
#include "MALLOC.h"
#if _MSC_VER
#include "strdup_windows.h"
#endif
#include "stricmp.h"

/*--------------------------------------------------------------------------*/
char **completion_generic(char **dictionary, int sizedictionary,
                          char *somechars, int *sizeArrayReturned)
{
    char **results = NULL;
    int nbElements = 0;
    int i = 0;

    for (i = 0; i < sizedictionary; i++)
    {
        if (dictionary[i])
        {
            if ( strnicmp(dictionary[i], somechars, strlen(somechars)) == 0)
            {
                nbElements++;
                /* +1 in MALLOC because a NULL element is inserted at the end of the array */
                /* This NULL element is used in Java wrapper to know the size of the array */
                if (results)
                {
                    results = (char**)REALLOC(results, sizeof(char*) * (nbElements + 1));
                }
                else
                {
                    results = (char**)MALLOC(sizeof(char*) * (nbElements + 1));
                }

                results[nbElements] = NULL; /* Last element set to NULL */
                results[nbElements - 1] = strdup(dictionary[i]);
            }
        }
    }
    *sizeArrayReturned = nbElements;
    return results;
}
/*--------------------------------------------------------------------------*/
