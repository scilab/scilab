/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 * Copyright (C) 2010-2012 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
#include "gw_core.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "do_error_number.h"
#include "freeArrayOfString.h"
#include "strsubst.h"
#include "MALLOC.h"

//#undef CheckRhs
//#undef CheckLhs
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
#define defaultErrorCode 10000
#define defaultErrorPosition 0
#define defaultErrorMessage " "
#define bufferErrorMessage C2F(cha1).buf
/*--------------------------------------------------------------------------*/
static int error_one_rhs(const char *fname);
static int error_two_rhs(const char *fname);
static int error_one_rhs_string(void *_pvCtx,
    int* _piAddressOne,
    const char *fname);
static int error_one_rhs_number(void *_pvCtx,
    int* _piAddressOne,
    const char *fname);
static int error_two_rhs_number_string(void *_pvCtx,
    int* _piAddressString, int iVarString,
    int* _piAddressNumber, int iVarNumber,
    const char *fname);
static int error_two_rhs_number_number(void *_pvCtx,
    int* _piAddressOne,
    int* _piAddressTwo,
    const char *fname);
static char *concatenateStrings(const char **pStrs, int nbElements);
static void setErrorMessage(const char *fname,
    int iErrorCode,
    int iErrorPosition,
    const char **pMessages,
    int nbElements);

/*--------------------------------------------------------------------------*/
int C2F(sci_error)(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (Rhs == 1)
    {
        return error_one_rhs(fname);
    }
    else /* Rhs == 2 */
    {
        return error_two_rhs(fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int error_one_rhs(const char *fname)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
    }
    else
    {
        if (isDoubleType(pvApiCtx, piAddressVarOne))
        {
            return error_one_rhs_number(pvApiCtx, piAddressVarOne,  fname);
        }
        else if (isStringType(pvApiCtx, piAddressVarOne))
        {
            return error_one_rhs_string(pvApiCtx, piAddressVarOne,  fname);
        }
        else
        {
            Scierror(999,_("%s: Wrong type for input argument #%d.\n"), fname, 1);
        }
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
static int error_two_rhs(const char *fname)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarOne) && isStringType(pvApiCtx, piAddressVarTwo))
    {
        return error_two_rhs_number_string(pvApiCtx, piAddressVarTwo, 2, piAddressVarOne, 1, fname);
    }
    else if (isDoubleType(pvApiCtx, piAddressVarTwo) && isStringType(pvApiCtx, piAddressVarOne))
    {
        return error_two_rhs_number_string(pvApiCtx, piAddressVarOne, 1, piAddressVarTwo, 2, fname);
    }
    else if (isDoubleType(pvApiCtx, piAddressVarTwo) && isDoubleType(pvApiCtx, piAddressVarOne))
    {
        return error_two_rhs_number_number(pvApiCtx, piAddressVarOne, piAddressVarTwo, fname);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument.\n"), fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int error_one_rhs_string(void *_pvCtx, 
    int* _piAddressOne,
    const char *fname)
{
    if (isRowVector(_pvCtx, _piAddressOne) || isColumnVector(_pvCtx, _piAddressOne) || isScalar(_pvCtx, _piAddressOne))
    {
        int m = 0;
        int n = 0;
        char **pStrs = NULL;

        if (getAllocatedMatrixOfString(_pvCtx, _piAddressOne, &m, &n, &pStrs) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        setErrorMessage(fname, defaultErrorCode, defaultErrorPosition, (const char **)pStrs, m * n);

        freeAllocatedMatrixOfString(m, n, pStrs);
        pStrs = NULL;
    }
    else
    {
        Scierror(999,_("%s: Wrong size for input argument #%d.\n"), fname, 1);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int error_one_rhs_number(void *_pvCtx,
    int* _piAddressOne,
    const char *fname)
{
    double dValue = 0.;
    int iErrorCode = 0;

    if (!isScalar(_pvCtx, _piAddressOne))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }

    if (getScalarDouble(_pvCtx, _piAddressOne, &dValue) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    iErrorCode = (int)dValue;
    if (dValue != (double)iErrorCode)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
        return 0;
    }

    if (iErrorCode <= 0)
    {
        Scierror(999 ,_("%s: Wrong value for input argument #%d: Value greater than 0 expected.\n"), fname, 1);
        return 0;
    }

    setErrorMessage(fname, iErrorCode, defaultErrorPosition, NULL, 0);

    return 0;
}
/*--------------------------------------------------------------------------*/
static int error_two_rhs_number_number(void *_pvCtx,
    int* _piAddressOne,
    int* _piAddressTwo,
    const char *fname)
{
    double dValue1 = 0.;
    double dValue2 = 0.;
    int iErrorCode = 0;
    int iErrorPosition = 0;

    if (!isScalar(_pvCtx, _piAddressOne))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }

    if (!isScalar(_pvCtx, _piAddressTwo))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 2);
        return 0;
    }

    if (getScalarDouble(_pvCtx, _piAddressOne, &dValue1) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    iErrorCode = (int)dValue1;
    if (dValue1 != (double)iErrorCode)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
        return 0;
    }

    if (getScalarDouble(_pvCtx, _piAddressTwo, &dValue2) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    iErrorPosition = (int)dValue2;
    if (dValue2 != (double)iErrorPosition)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
        return 0;
    }

    if (iErrorCode <= 0)
    {
        Scierror(999 ,_("%s: Wrong value for input argument #%d: Value greater than 0 expected.\n"), fname, 1);
        return 0;
    }

    if (iErrorPosition <= 0)
    {
        Scierror(999 ,_("%s: Wrong value for input argument #%d: Value greater than 0 expected.\n"), fname, 2);
        return 0;
    }

    setErrorMessage(fname, iErrorCode, iErrorPosition, NULL, 0);
    return 0;
}
/*--------------------------------------------------------------------------*/
static int error_two_rhs_number_string(void *_pvCtx,
    int* _piAddressString, int iVarString,
    int* _piAddressNumber, int iVarNumber,
    const char *fname)
{
    double dValue = 0.;
    int iErrorCode = 0;

    if (!isScalar(_pvCtx, _piAddressNumber))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, iVarNumber);
        return 0;
    }

    if (getScalarDouble(_pvCtx, _piAddressNumber, &dValue) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    iErrorCode = (int)dValue;
    if (dValue != (double)iErrorCode)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, iVarNumber);
        return 0;
    }

    if (iErrorCode <= 0)
    {
        Scierror(999 ,_("%s: Wrong value for input argument #%d: Value greater than 0 expected.\n"), fname, iVarNumber);
        return 0;
    }

    if (isRowVector(_pvCtx, _piAddressString)  || isColumnVector(_pvCtx, _piAddressString) || isScalar(_pvCtx, _piAddressString))
    {
        int m = 0;
        int n = 0;
        char **pStrs = NULL;

        if (getAllocatedMatrixOfString(_pvCtx, _piAddressString, &m, &n, &pStrs) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        setErrorMessage(fname, iErrorCode, defaultErrorPosition, (const char**)pStrs, m * n);

        freeAllocatedMatrixOfString(m, n, pStrs);
        pStrs = NULL;
    }
    else
    {
        Scierror(999,_("%s: Wrong size for input argument #%d.\n"), fname, iVarString);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static void setErrorMessage(const char *fname,
    int iErrorCode,
    int iErrorPosition,
    const char **pMessages,
    int nbElements)
{
    /* empty Fortran buffer */
    memset(bufferErrorMessage, 0, sizeof(char) * bsiz);

    if (pMessages && nbElements > 0)
    {
        char *concatenatedString = concatenateStrings(pMessages, nbElements);
        char *msg = strsub(concatenatedString, "%", "%%");
        C2F(iop).err = defaultErrorPosition;
        if (msg)
        {
            Scierror(iErrorCode, "%s\n", msg);
            FREE(msg);
            msg = NULL;
        }
        else
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
        }

        if (concatenatedString)
        {
            FREE(concatenatedString);
            concatenatedString = NULL;
        }
    }
    else
    {
        strcpy(bufferErrorMessage, defaultErrorMessage);
        C2F(iop).err = iErrorPosition;
        SciError(iErrorCode);
    }
}
/*--------------------------------------------------------------------------*/
static char *concatenateStrings(const char **pStrs, int nbElements)
{
    char *concatenatedString = NULL;
    size_t szLength = 0;
    int i = 0;
    if (pStrs == NULL)
    {
        return concatenatedString;
    }

    for (i = 0; i < nbElements; i++)
    {
        szLength = szLength + strlen(pStrs[i]) + strlen("\n") + 1;
    }

    concatenatedString = (char *)MALLOC(sizeof(char) * szLength);
    if (concatenatedString == NULL)
    {
        return concatenatedString;
    }

    memset(concatenatedString, 0, sizeof(char) * szLength);
    for (i = 0; i < nbElements; i++)
    {
        if (i == 0)
        {
            strcpy(concatenatedString, pStrs[i]);
        }
        else
        {
            strcat(concatenatedString, pStrs[i]);
        }
        if (i < nbElements - 1) strcat(concatenatedString, "\n");
    }
    return concatenatedString;
}
/*--------------------------------------------------------------------------*/

