/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

/*------------------------------------------------------------------------*/
/* file: GetHashTable.c                                                   */
/* desc : implementation of the scilab hashtable for the get procedure    */
/*------------------------------------------------------------------------*/
#include "GetHashTable.h"
#include "Scierror.h"
#include "localization.h"
#include "getDictionaryGetProperties.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "setGetHashTable.h"


void* callGetProperty(void* _pvCtx, int iObjUID, char *propertyName)
{
    getPropertyFunc accessor = searchGetHashtable(propertyName);

    if (accessor == NULL)
    {
        Scierror(999, _("Unknown property: %s.\n"), propertyName);
        return NULL;
    }
    return accessor(_pvCtx, iObjUID);
}

/*--------------------------------------------------------------------------*/
char **getDictionaryGetProperties(int *sizearray)
{
    char **dictionary = NULL;
    size_t propertyCount = 0;
    const GetPropertyEntry* entries = getGetPropertyEntries(&propertyCount);

    *sizearray = 0;

    dictionary = (char **)MALLOC(sizeof(char *) * propertyCount);
    if (dictionary)
    {
        size_t i = 0;

        *sizearray = (int)propertyCount;
        for (i = 0; i < propertyCount ; i++)
        {
            dictionary[i] = os_strdup(entries[i].key);
        }
    }
    return dictionary;
}
/*--------------------------------------------------------------------------*/
