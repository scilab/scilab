/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Sylvestre LEDRU
* Copyright (C) 2009-2012 - DIGITEO - Allan CORNET
* Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
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
#include <stdlib.h>
#include "sci_malloc.h"
#include "localization.h"
#include "gw_localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "strsubst.h"
#include "os_string.h"
#include "freeArrayOfString.h"

/*--------------------------------------------------------------------------*/
static char *convertString_dgettext(const char *domain, const char *pStr);
/*--------------------------------------------------------------------------*/
int sci_gettext(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    char* pstDomain = NULL;
    char **TranslatedStrings = NULL;
    int m = 0;
    int n = 0;
    char **StringsToTranslate = NULL;
    int i = 0;

    int iCurrentRhs = 1;

    int iRhs = nbInputArgument(pvApiCtx);
    int iLhs = nbOutputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (iRhs == 2)
    {
        //get domain name
        sciErr = getVarAddressFromPosition(pvApiCtx, iCurrentRhs, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iCurrentRhs);
        }

        if (isStringType(pvApiCtx, piAddressVarOne) == 0 || isScalar(pvApiCtx, piAddressVarOne) == 0)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname, iCurrentRhs);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &pstDomain))
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        iCurrentRhs++;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, iCurrentRhs, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iCurrentRhs);
    }

    if (isStringType(pvApiCtx, piAddressVarOne) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        freeAllocatedSingleString(pstDomain);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarOne, &m, &n, &StringsToTranslate) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        freeAllocatedSingleString(pstDomain);
        return 0;
    }

    TranslatedStrings = (char **)MALLOC(sizeof(char*) * (m * n));
    if (TranslatedStrings == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        freeAllocatedSingleString(pstDomain);
        freeAllocatedMatrixOfString(m, n, StringsToTranslate);
        StringsToTranslate = NULL;
        return 0;
    }

    for (i = 0; i < m * n; i++)
    {
        if (strcmp(StringsToTranslate[i], "") == 0)
        {
            TranslatedStrings[i] = strdup("");
        }
        else
        {
            //if pstDomain is NULL, default domain will be use
            TranslatedStrings[i] = convertString_dgettext(pstDomain, StringsToTranslate[i]);
        }
    }

    freeAllocatedSingleString(pstDomain);
    freeAllocatedMatrixOfString(m, n, StringsToTranslate);
    StringsToTranslate = NULL;

    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, m, n, TranslatedStrings);
    freeArrayOfString(TranslatedStrings, m * n);
    TranslatedStrings = NULL;

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static char *convertString_dgettext(const char *domain, const char *pStr)
{
    char *pStrConverted = NULL;

    if (pStr)
    {
        BOOL revertStrsub = FALSE;
        char *TranslatedString = NULL;
        char *tmpStr = NULL;
        if (strchr(pStr, '\\') != NULL)
        {
            char *tmpStr1 = NULL;
            char *tmpStr2 = NULL;
            /* There is an \ in the string process to replace */

            /* We always have something from this functions because gettext
            * is returning the same string if it cannot find it */

            tmpStr1 = strsub((char*)pStr, "\\n", "\n"); /* linefeed */
            tmpStr2 = strsub(tmpStr1, "\\t", "\t"); /* horizontal tab */
            if (tmpStr1)
            {
                FREE(tmpStr1);
                tmpStr1 = NULL;
            }

            tmpStr1 = strsub(tmpStr2, "\\r", "\r"); /* carriage return */
            if (tmpStr2)
            {
                FREE(tmpStr2);
                tmpStr2 = NULL;
            }

            tmpStr2 = strsub(tmpStr1, "\\v", "\v"); /* vertical tab */
            if (tmpStr1)
            {
                FREE(tmpStr1);
                tmpStr1 = NULL;
            }

            tmpStr1 = strsub(tmpStr2, "\\f", "\f"); /* form feed */
            if (tmpStr2)
            {
                FREE(tmpStr2);
                tmpStr2 = NULL;
            }

            tmpStr2 = strsub(tmpStr1, "\\\\", "\\"); /* backslash */
            if (tmpStr1)
            {
                FREE(tmpStr1);
                tmpStr1 = NULL;
            }

            tmpStr1 = strsub(tmpStr2, "\\\"", "\""); /* double quote */
            if (tmpStr2)
            {
                FREE(tmpStr2);
                tmpStr2 = NULL;
            }

            revertStrsub = TRUE;
            tmpStr = strdup(tmpStr1);
            if (tmpStr1)
            {
                FREE(tmpStr1);
                tmpStr1 = NULL;
            }
        }
        else
        {
            revertStrsub = FALSE;
            tmpStr = strdup(pStr);
        }

        TranslatedString = strdup(dgettext(domain, tmpStr));
        if (tmpStr)
        {
            FREE(tmpStr);
            tmpStr = NULL;
        }

        /* Add removed slashes */
        if (revertStrsub)
        {
            char *tmpStr1 = NULL;
            char *tmpStr2 = NULL;

            tmpStr1 = strsub(TranslatedString, "\\", "\\\\"); /* backslash */

            tmpStr2 = strsub(tmpStr1, "\f", "\\f"); /* form feed */
            if (tmpStr1)
            {
                FREE(tmpStr1);
                tmpStr1 = NULL;
            }

            tmpStr1 = strsub(tmpStr2, "\n", "\\n"); /* linefeed */
            if (tmpStr2)
            {
                FREE(tmpStr2);
                tmpStr2 = NULL;
            }

            tmpStr2 = strsub(tmpStr1, "\t", "\\t"); /* horizontal tab */
            if (tmpStr1)
            {
                FREE(tmpStr1);
                tmpStr1 = NULL;
            }

            tmpStr1 = strsub(tmpStr2, "\r", "\\r"); /* carriage return */
            if (tmpStr2)
            {
                FREE(tmpStr2);
                tmpStr2 = NULL;
            }

            tmpStr2 = strsub(tmpStr1, "\v", "\\v"); /* vertical tab */
            if (tmpStr1)
            {
                FREE(tmpStr1);
                tmpStr1 = NULL;
            }

            if (TranslatedString)
            {
                FREE(TranslatedString);
                TranslatedString = NULL;
            }

            TranslatedString = strdup(tmpStr2);

            if (tmpStr2)
            {
                FREE(tmpStr2);
                tmpStr2 = NULL;
            }
        }
        pStrConverted = TranslatedString;
    }
    return pStrConverted;
}
/*--------------------------------------------------------------------------*/
