/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "stringsstrrchr.h"
/*----------------------------------------------------------------------------*/
static wchar_t **getInputArgumentAsMatrixOfWideString(void* _pvCtx, int _iVar,
                                       const char *fname,
                                       int *m, int *n, int *iErr);
/*----------------------------------------------------------------------------*/
int sci_strchr(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iErr = 0;

    wchar_t **pStrVarOne = NULL;
    int m1 = 0; int n1 = 0;

    wchar_t **pStrVarTwo = NULL;
    int m2 = 0; int n2 = 0;

    int i = 0;
    BOOL do_strchr = (strcmp(fname, "strchr") == 0);

    wchar_t **wcOutput_Strings = NULL;

    pStrVarOne = getInputArgumentAsMatrixOfWideString(pvApiCtx, 1, fname, &m1, &n1, &iErr);
    if (iErr) return 0;

    pStrVarTwo = getInputArgumentAsMatrixOfWideString(pvApiCtx, 2, fname, &m2, &n2, &iErr);
    if (iErr) 
    {
        if (pStrVarOne) {freeAllocatedMatrixOfWideString(m1, n1, pStrVarOne); pStrVarOne = NULL;}
        return 0;
    }

    for (i = 0; i < m2 * n2; i++)
    {
		if (wcslen(pStrVarTwo[i]) != 1)
        {
            if (pStrVarOne) {freeAllocatedMatrixOfWideString(m1, n1, pStrVarOne); pStrVarOne = NULL;}
            if (pStrVarTwo) {freeAllocatedMatrixOfWideString(m2, n2, pStrVarTwo); pStrVarTwo = NULL;}
            Scierror(999, _("%s: Wrong size for input argument #%d: A character expected.\n"), fname, 2);
            return 0;
        }
    }

    wcOutput_Strings = strings_wcsrchr((const wchar_t**)pStrVarOne, m1 * n1, (const wchar_t**)pStrVarTwo, m2 * n2, do_strchr);
    if (pStrVarOne) {freeAllocatedMatrixOfWideString(m1, n1, pStrVarOne); pStrVarOne = NULL;}
    if (pStrVarTwo) {freeAllocatedMatrixOfWideString(m2, n2, pStrVarTwo); pStrVarTwo = NULL;}

    if (wcOutput_Strings)
    {
        sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 1 , m1, n1, wcOutput_Strings);

        if (wcOutput_Strings) {freeAllocatedMatrixOfWideString(m1, n1, wcOutput_Strings); wcOutput_Strings = NULL;}

        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(1) = Rhs+1 ;
        PutLhsVar();
    }
    else
    {
        Scierror(999,_("%s: No more memory.\n"), fname);
    }
    return 0;
}
/*----------------------------------------------------------------------------*/
static wchar_t **getInputArgumentAsMatrixOfWideString(void* _pvCtx, int _iVar,
                                       const char *fname,
                                       int *m, int *n, int *iErr)
{
    SciErr sciErr;
    wchar_t **pStringValues = NULL;
    int *piAddressVar = NULL;
    int m_ = 0, n_ = 0;
    int iType = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, _iVar, &piAddressVar);
    if(sciErr.iErr)
    {
        *iErr = sciErr.iErr;
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, _iVar);
        return NULL;
    }

    sciErr = getVarType(pvApiCtx, piAddressVar, &iType);
    if(sciErr.iErr)
    {
        *iErr = sciErr.iErr;
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, _iVar);
        return NULL;
    }

    if (iType != sci_strings)
    {
        *iErr =  API_ERROR_INVALID_TYPE;
        Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, _iVar);
        return NULL;
    }

    *iErr = getAllocatedMatrixOfWideString(pvApiCtx, piAddressVar, &m_, &n_, &pStringValues);
    if (*iErr)
    {
        if (pStringValues) {freeAllocatedMatrixOfWideString(m_, n_, pStringValues); pStringValues = NULL;}
        *m = 0;
        *n = 0;
        Scierror(999,_("%s: Memory allocation error.\n"), fname);
    }
    else
    {
        *m = m_;
        *n = n_;
    }

    return pStringValues;
}
/*----------------------------------------------------------------------------*/
