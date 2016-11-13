/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010-2011 - Calixte DENIZET
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

#include "context.hxx"
#include "struct.hxx"
#include "tlist.hxx"

extern "C"
{
#include <string.h> /* strcmp */
#include <stdlib.h> /* qsort */
#include "Scierror.h"
#include "api_scilab.h"
#include "sci_malloc.h"
#include "getfieldsdictionary.h"
#include "getPartLine.h"
#include "completion.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
#include "getfields.h"
}

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
    char** pstData = NULL;
    char **fields = NULL;
    char *pstVar = NULL;
    int iXlist = 0;

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

    FREE(lineBeforePoint);
    lineBeforePoint = NULL;

    symbol::Context* pCtx = symbol::Context::getInstance();

    types::InternalType* pIT = pCtx->get(symbol::Symbol(pstVar));
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

        pstData = pFields->get();
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

        pstData = pFields->get();
        iXlist = 1;
    }
    else
    {
        return NULL;
    }

    int iLast = 0;
    char** _fields = (char**)MALLOC(sizeof(char*) * (iSize + 1));
    for (int i = iXlist; i < (iSize + iXlist); ++i)
    {
        if (strstr(pstData[i], pattern) == pstData[i])
        {
            _fields[iLast++] = os_strdup(pstData[i]);
        }

    }

    _fields[iLast] = NULL;
    *size = iLast;
    qsort(_fields, *size, sizeof(char*), cmpNames);

    return _fields;
}
/*--------------------------------------------------------------------------*/
