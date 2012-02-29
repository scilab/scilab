/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Sylvestre LEDRU
* Copyright (C) 2012 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "localization.h"
#include "gw_localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "strsubst.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
static char *convertString_dgettext(const char *domain, const char *pStr);
/*--------------------------------------------------------------------------*/
int sci_dgettext(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    CheckRhs(2, 2);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
    }

    if (isStringType(pvApiCtx, piAddressVarOne) && isStringType(pvApiCtx, piAddressVarTwo))
    {
        char *domain = NULL;

        if (!isScalar(pvApiCtx, piAddressVarOne))
        {
            Scierror(999,"%s: Wrong size for input argument #%d: A string expected.\n", fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &domain) == 0)
        {
            char **stringsToTranslate = NULL;
            char **TranslatedStrings = NULL;
            int m = 0, n = 0;
            int i = 0;

            if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarTwo, &m, &n, &stringsToTranslate) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            TranslatedStrings = (char **)MALLOC(sizeof(char*) * (m * n));
            if (TranslatedStrings == NULL)
            {
                freeAllocatedMatrixOfString(m, n, stringsToTranslate);
                stringsToTranslate = NULL;
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            for (i = 0; i < m * n; i++)
            {
                if (strcmp(stringsToTranslate[i], "") == 0)
                {
                    TranslatedStrings[i] = strdup("");
                }
                else
                {
                    TranslatedStrings[i] = convertString_dgettext(domain, stringsToTranslate[i]);
                }
            }

            freeAllocatedMatrixOfString(m, n, stringsToTranslate);
            stringsToTranslate = NULL;

            sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, m, n, TranslatedStrings);
            freeArrayOfString(TranslatedStrings, m * n);
            TranslatedStrings = NULL;

            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999,_("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
        else
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }
    else
    {
        if (isStringType(pvApiCtx, piAddressVarOne))
        {
            Scierror(999,"%s: Wrong type for input argument #%d: String expected.\n", fname, 2);
        }
        else
        {
            Scierror(999,"%s: Wrong type for input argument #%d: String expected.\n", fname, 1);
        }
    }
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
