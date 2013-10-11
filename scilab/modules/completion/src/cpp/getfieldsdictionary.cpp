/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010-2011 - Calixte DENIZET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

extern "C"
{
#include <string.h> /* strcmp */
#include <stdlib.h> /* qsort */
#include "Scierror.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "getfieldsdictionary.h"
#include "getPartLine.h"
#include "completion.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
#include "getfields.h"
}

#include "context.hxx"
#include "struct.hxx"
#include "tlist.hxx"

static int isInitialized = 0;

/*--------------------------------------------------------------------------*/
static int cmpNames(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}
/*--------------------------------------------------------------------------*/
char **getfieldsdictionary(char *lineBeforeCaret, char *pattern, int *size)
{
    int rc = 0;
    char **pstData = NULL;
    char **fields = NULL;
    char *pstVar = NULL;
    wchar_t* pwstVar = NULL;

    char *lineBeforePoint = NULL;
    int pos = (int)(strlen(lineBeforeCaret) - strlen(pattern) - 1);

    if (!isInitialized)
    {
        initializeFieldsGetter();
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
    pstVar = getPartLevel(lineBeforePoint);

    pwstVar = to_wide_string(pstVar);
    FREE(pstVar);

    FREE(lineBeforePoint);
    lineBeforePoint = NULL;

    symbol::Context* pCtx = symbol::Context::getInstance();

    types::InternalType* pIT = pCtx->get(symbol::Symbol(pwstVar));
    FREE(pwstVar);
    if (pIT == NULL)
    {
        return NULL;
    }

    //cells are containers but there are no fields
    if (pIT->isContainer() == false || pIT->isCell())
    {
        return NULL;
    }

    if (pIT->isHandle())
    {
        return completionOnHandleGraphicsProperties(pattern, size);
    }

    types::String* pFields = NULL;
    int iSize = 0;
    if (pIT->isStruct())
    {
        types::Struct* pStr = pIT->getAs<types::Struct>();
        pFields = pStr->getFieldNames();
        if (pFields == 0)
        {
            return NULL;
        }

        iSize = pFields->getSize();

        pstData = (char**)MALLOC(sizeof(char*) * iSize);

        for (int i = 0 ; i < iSize ; i++)
        {
            pstData[i] = wide_string_to_UTF8(pFields->get(i));
        }
    }
    else if (pIT->isTList() || pIT->isMList())
    {
        types::TList* pL = pIT->getAs<types::TList>();
        pFields = pL->getFieldNames();

        //bypass the value, is the (t/m)list type
        iSize = pFields->getSize() - 1;
        if (iSize == 0)
        {
            return NULL;
        }

        pstData = (char**)MALLOC(sizeof(char*) * iSize);

        for (int i = 0 ; i < iSize ; i++)
        {
            pstData[i] = wide_string_to_UTF8(pFields->get(i + 1));
        }
    }
    else
    {
        return NULL;
    }

    int iFieldsSize = 0;
    fields = (char**)getFieldsForType(pstData[0], NULL, NULL, 0, &iFieldsSize);
    if (fields)
    {
        freeArrayOfString(pstData, rc);
        pstData = fields;
        for (rc = 0; fields[rc]; rc++)
        {
            ;
        }
    }

    int iLast = 0;
    for (int i = 0 ; i < iSize ; i++)
    {
        if (strstr(pstData[i], pattern) != pstData[i])
        {
            FREE(pstData[i]);
            pstData[i] = NULL;
        }
        else if (pstData[iLast] != pstData[i])
        {
            pstData[iLast] = pstData[i];
            pstData[i] = NULL;
            iLast++;
        }
        else
        {
            iLast++;
        }
    }

    *size = iLast;
    qsort(pstData, *size, sizeof(char*), cmpNames);

    return pstData;
}
/*--------------------------------------------------------------------------*/
