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
#include "completion_generic.h"
#include "sci_malloc.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
char **completion_generic(char **dictionary, int sizedictionary,
                          const char *somechars, int *sizeArrayReturned)
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
                results[nbElements - 1] = os_strdup(dictionary[i]);
            }
        }
    }
    *sizeArrayReturned = nbElements;
    return results;
}
/*--------------------------------------------------------------------------*/
