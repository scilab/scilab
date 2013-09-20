/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010-2011 - Calixte DENIZET
* Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
#include <string.h> /* strcmp */
#include <stdlib.h> /* qsort */
#include "Scierror.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "getfieldsdictionary.h"
#include "getPartLine.h"
#include "completion.h"
#include "freeArrayOfString.h"

#include "getfields.h"

static int isInitialized = 0;

/*--------------------------------------------------------------------------*/
static int cmpNames(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}
/*--------------------------------------------------------------------------*/
char **getfieldsdictionary(char *lineBeforeCaret, char *pattern, int *size)
{
    SciErr sciErr;
    int *piAddr = NULL;
    int *piLen = NULL;
    int piType = 0;
    int rows = 0;
    int cols = 0;
    int rc = 0;
    int i;
    int last = 0;
    char **pstData = NULL;
    char **fields = NULL;
    char **_fields = NULL;
    char *var = NULL;
    char *lineBeforePoint = NULL;
    int pos = (int)(strlen(lineBeforeCaret) - strlen(pattern) - 1);
    char ** fieldPath = NULL;
    int fieldPathLen = 0;
    int fieldCompt = 0;
    int fieldsSize = 0;

    if (!isInitialized)
    {
        initializeFieldsGetter(1);
        isInitialized = 1;
    }

    if (pos <= 0 || lineBeforeCaret[pos] != '.')
    {
        return NULL;
    }

    lineBeforePoint = (char*)MALLOC(sizeof(char) * (pos + 1));
    if (lineBeforePoint == NULL)
    {
        return NULL;
    }
    memcpy(lineBeforePoint, lineBeforeCaret, pos);
    lineBeforePoint[pos] = '\0';
    fieldPath = getFieldPath(lineBeforePoint, &fieldPathLen);
    if (fieldPathLen == 0)
    {
        return NULL;
    }

    FREE(lineBeforePoint);
    lineBeforePoint = NULL;

    sciErr = getNamedVarType(pvApiCtx, fieldPath[0], &piType);
    if (sciErr.iErr && piType != sci_mlist && piType != sci_tlist && piType != sci_handles)
    {
        freeArrayOfString(fieldPath, fieldPathLen);
        return NULL;
    }

    if (piType == sci_mlist || piType == sci_tlist || piType == sci_handles)
    {
        getVarAddressFromName(pvApiCtx, fieldPath[0], &piAddr);
        if (sciErr.iErr)
        {
            freeArrayOfString(fieldPath, fieldPathLen);
            return NULL;
        }

        fields = (char**)getFields(piAddr, fieldPath, fieldPathLen, &fieldsSize);
        freeArrayOfString(fieldPath, fieldPathLen);
        if (!fields)
        {
            return NULL;
        }

        _fields = (char**)MALLOC(sizeof(char *) * (fieldsSize + 1));
        last = 0;

        for (i = 0; i < fieldsSize ; i++)
        {
            if (strstr(fields[i], pattern) == fields[i])
            {
                _fields[last++] = fields[i];
            }
            else
            {
                FREE(fields[i]);
                fields[i] = NULL;
            }
        }

        FREE(fields);

        *size = last;
        qsort(_fields, *size, sizeof(char*), cmpNames);
        _fields[last] = NULL; // don't forget, it SWIG is using first NULL item to guess array size

        return _fields;
    }
    else
    {
        pstData = completionOnHandleGraphicsProperties(pattern, size);
    }

    return pstData;
}
/*--------------------------------------------------------------------------*/
