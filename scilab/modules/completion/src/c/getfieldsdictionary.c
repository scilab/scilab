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
#include <string.h> /* strcmp */
#include <stdlib.h> /* qsort */
#include "Scierror.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "getfieldsdictionary.h"
#include "getPartLine.h"
#include "completion.h"
#include "freeArrayOfString.h"
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
    int nbItem = 0;
    int rows = 0;
    int cols = 0;
    int rc = 0;
    int i;
    int last = 0;
    char **pstData = NULL;
    char *var = NULL;
    char *lineBeforePoint = NULL;
    int pos = (int)(strlen(lineBeforeCaret) - strlen(pattern) - 1);

    if (pos <= 0 || lineBeforeCaret[pos] != '.')
    {
        return NULL;
    }

    lineBeforePoint = (char*)MALLOC(sizeof(char) * (pos + 1));
    if (lineBeforePoint == NULL) return NULL;
    memcpy(lineBeforePoint, lineBeforeCaret, pos);
    lineBeforePoint[pos] = '\0';
    var = getPartLevel(lineBeforePoint);
    FREE(lineBeforePoint);
    lineBeforePoint = NULL;

    sciErr = getNamedVarType(pvApiCtx, var, &piType);
    if (sciErr.iErr && piType != sci_mlist && piType != sci_tlist && piType != sci_handles)
    {
        FREE(var);
        var = NULL;
        return NULL;
    }

    if (piType == sci_mlist || piType == sci_tlist)
    {
        getVarAddressFromName(pvApiCtx, var, &piAddr);
        FREE(var);
        var = NULL;
        if (sciErr.iErr)
        {
            return NULL;
        }

        sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 1, &rows, &cols, NULL, NULL);
        if (sciErr.iErr)
        {
            return NULL;
        }

        rc = rows * cols;
        if (rc == 1)
        {
            return NULL;
        }

        piLen = (int*)MALLOC(sizeof(int) * rc);
        sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 1, &rows, &cols, piLen, NULL);
        if (sciErr.iErr)
        {
            FREE(piLen);
            return NULL;
        }

        pstData = (char**)MALLOC(sizeof(char*) * (rc + 1));
        pstData[rc] = NULL;
        for (i = 0 ; i < rc ; i++)
        {
            pstData[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));
        }

        sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 1, &rows, &cols, piLen, pstData);
        if (sciErr.iErr)
        {
            FREE(piLen);
            piLen = NULL;
            freeArrayOfString(pstData, rc);
            return NULL;
        }
        FREE(piLen);

        // We remove all the entries which don't begin with fieldpart
        // and the first entry (and the second if it is a struct)
        if (!strcmp(pstData[0], "st"))
        {
            FREE(pstData[0]);
            pstData[0] = NULL;
            FREE(pstData[1]);
            pstData[1] = NULL;
            if (rc == 2)
            {
                FREE(pstData);
                return NULL;
            }
            i = 2;
        }
        else
        {
            FREE(pstData[0]);
            pstData[0] = NULL;
            i = 1;
        }

        for (; i < rc ; i++)
        {
            if (strstr(pstData[i], pattern) != pstData[i])
            {
                FREE(pstData[i]);
                pstData[i] = NULL;
            }
            else
            {
                pstData[last] = pstData[i];
                pstData[i] = NULL;
                last++;
            }
        }

        *size = last;
        qsort(pstData, *size, sizeof(char*), cmpNames);
    }
    else
    {
        pstData = completionOnHandleGraphicsProperties(pattern, size);
    }

    return pstData;
}
/*--------------------------------------------------------------------------*/
