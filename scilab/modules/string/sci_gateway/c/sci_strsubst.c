/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU , Allan CORNET
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_string.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "freeArrayOfString.h"
#include "strsubst.h"
#include "localization.h"
#include "Scierror.h"
#include "pcre_error.h"
#include "pcre_private.h"
#include "BOOL.h"
/*-------------------------------------------------------------------------------------*/
#define CHAR_R "r"
#define CHAR_S "s"
/*-------------------------------------------------------------------------------------*/
int sci_strsubst(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    char **pStVarOne = NULL;
    int mOne = 0, nOne = 0;

    int *piAddressVarTwo = NULL;
    char *pStVarTwo = NULL;

    int *piAddressVarThree = NULL;
    char *pStVarThree = NULL;

    char** Output_StringMatrix = NULL;

    BOOL isRegExp = FALSE;


    CheckRhs(3, 4);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne) == 0 && isEmptyMatrix(pvApiCtx, piAddressVarOne) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname);
        return 0;
    }

    //to manage []
    if (isEmptyMatrix(pvApiCtx, piAddressVarOne))
    {
        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, mOne, nOne, NULL);
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


    if (Rhs == 4)
    {
        int *piAddressVarFour = NULL;
        char *pStVarFour = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarFour) == 0 || isScalar(pvApiCtx, piAddressVarFour) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 4);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarFour, &pStVarFour))
        {
            if (pStVarFour)
            {
                freeAllocatedSingleString(pStVarFour);
            }

            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if ((strcmp(pStVarFour, CHAR_R) == 0) || (strcmp(pStVarFour, CHAR_S) == 0))
        {
            if (strcmp(pStVarFour, CHAR_R) == 0)
            {
                isRegExp = TRUE;
            }
            else
            {
                isRegExp = FALSE;
            }

            FREE(pStVarFour);
        }
        else
        {
            FREE(pStVarFour);
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 4, "s", "r");
            return 0;
        }
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarThree) == 0 || isScalar(pvApiCtx, piAddressVarThree) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarTwo) == 0 || isScalar(pvApiCtx, piAddressVarTwo) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarThree, &pStVarThree))
    {
        freeAllocatedSingleString(pStVarThree);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &pStVarTwo))
    {
        freeAllocatedSingleString(pStVarTwo);
        freeAllocatedSingleString(pStVarThree);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarOne, &mOne, &nOne, &pStVarOne))
    {
        freeAllocatedMatrixOfString(mOne, nOne, pStVarOne);
        freeAllocatedSingleString(pStVarTwo);
        freeAllocatedSingleString(pStVarThree);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isRegExp)
    {
        int ierr = (int)PCRE_FINISHED_OK;

        Output_StringMatrix = strsubst_reg(pStVarOne, mOne * nOne, pStVarTwo, pStVarThree, &ierr);
        if ((ierr != PCRE_FINISHED_OK) && (ierr != NO_MATCH) && (ierr != PCRE_EXIT))
        {
            if (Output_StringMatrix)
            {
                freeArrayOfString(Output_StringMatrix, mOne * nOne);
            }

            freeAllocatedMatrixOfString(mOne, nOne, pStVarOne);
            freeAllocatedSingleString(pStVarTwo);
            freeAllocatedSingleString(pStVarThree);
            pcre_error(fname, ierr);
            return 0;
        }
    }
    else
    {
        Output_StringMatrix = strsubst(pStVarOne, mOne * nOne, pStVarTwo, pStVarThree);
    }

    freeAllocatedSingleString(pStVarThree);
    freeAllocatedSingleString(pStVarTwo);
    freeAllocatedMatrixOfString(mOne, nOne, pStVarOne);

    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, mOne, nOne, Output_StringMatrix);
    freeArrayOfString(Output_StringMatrix, mOne * nOne);
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

/*-------------------------------------------------------------------------------------*/
