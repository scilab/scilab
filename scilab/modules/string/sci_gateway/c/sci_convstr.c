/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/* desc : converts the matrix of strings  str-matrix  into lower case (for*/
/*          "l"  ;default value) or upper case (for  "u" ).               */
/*        [y]=convstr(str-matrix, ["flag"])                               */
/*              str-matrix, y : matrices of strings                       */
/*              "flag"  : string ( "u"  for upper or  "l"  for lower      */
/*                        (default value))                                */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "wchar.h"
#include "convstr.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*----------------------------------------------------------------------------*/
int sci_convstr(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    wchar_t **pstInput = NULL;
    wchar_t **pstOutput = NULL;
    char cType = LOW;
    int iRows1 = 0;
    int iCols1 = 0;
    int i = 0;

    int* piAddr1 = NULL;

    int* piAddr2 = NULL;

    int iRhs = nbInputArgument(pvApiCtx);
    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);


    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }


    if (isEmptyMatrix(pvApiCtx, piAddr1))
    {
        if (createEmptyMatrix(pvApiCtx, Rhs + 1))
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1 ;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (iRhs == 2)
    {
        char* pstFlag = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddr2) && isScalar(pvApiCtx, piAddr2) == 0)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        /* To put "flag" into typ; whether "u" or "l" */
        if (getAllocatedSingleString(pvApiCtx, piAddr2, &pstFlag))
        {
            freeAllocatedSingleString(pstFlag);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        if (strlen(pstFlag) != 1 || (pstFlag[0] != UPPER && pstFlag[0] != LOW && pstFlag[0] != UPPER_B && pstFlag[0] != LOW_B ))
        {
            freeAllocatedSingleString(pstFlag);
            Scierror(999, _("%s: Wrong value for input argument #%d: 'u' (Upper) or 'l' (Lower) expected.\n"), fname, 2);
            return 0;
        }

        cType = pstFlag[0];
        freeAllocatedSingleString(pstFlag);
    }

    if (isStringType(pvApiCtx, piAddr1) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 1);
        return 0;
    }

    /* To input the string matrix */
    if (getAllocatedMatrixOfWideString(pvApiCtx, piAddr1, &iRows1, &iCols1, &pstInput))
    {
        freeAllocatedMatrixOfWideString(iRows1, iCols1, pstInput);
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 1);
        return 0;
    }

    pstOutput = (wchar_t**)MALLOC(sizeof(wchar_t*) * iRows1 * iCols1);
    if (pstOutput == NULL)
    {
        freeAllocatedMatrixOfWideString(iRows1, iCols1, pstInput);
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < iRows1 * iCols1; i++)
    {
        pstOutput[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(pstInput[i]) + 1));
        if (pstOutput[i] == NULL)
        {
            freeAllocatedMatrixOfWideString(iRows1, iCols1, pstInput);
            freeArrayOfWideString(pstOutput, i);
            Scierror(999, ("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    /* convstr algorithm */
    convstr(pstInput, pstOutput, cType, iRows1 * iCols1);
    freeAllocatedMatrixOfWideString(iRows1, iCols1, pstInput);

    /* put on scilab stack */
    sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 1, iRows1, iCols1, (const wchar_t**)pstOutput);
    freeArrayOfWideString(pstOutput, iRows1 * iCols1);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1 ;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
