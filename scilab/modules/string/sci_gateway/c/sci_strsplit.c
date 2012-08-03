/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU , Allan CORNET
 * Copyright (C) DIGITEO - 2009-2011 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_string.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "BOOL.h"
#include "api_scilab.h"
#include "strsplitfunction.h"
/*----------------------------------------------------------------------------*/
static BOOL isValidRegExp(wchar_t *pStr);
/*----------------------------------------------------------------------------*/
int sci_strsplit(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int lw = 0;
    int *piAddressVarOne = NULL;
    wchar_t *pStVarOne = NULL;

    /* Check Input & Output parameters */
    CheckRhs(1, 3);
    CheckLhs(1, 2);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isEmptyMatrix(pvApiCtx, piAddressVarOne))
    {
        createEmptyMatrix(pvApiCtx, Rhs + 1);
        LhsVar(1) = Rhs + 1;
        if (Lhs == 2)
        {
            // [r1, r2] = strsplit([]);
            // r1 eq []
            // r2 eq ''
            createSingleString(pvApiCtx, Rhs + 2, "");
            LhsVar(2) = Rhs + 2;
        }
        PutLhsVar();
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 1);
        return 0;
    }


    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), fname, 1);
        return 0;
    }

    if (Rhs > 2) /* Rhs eq. 3 */
    {
        int *piAddressVarThree = NULL;
        double dValueThree = 0.;
        int iValueThree = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (!isScalar(pvApiCtx, piAddressVarThree))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 3);
            return 0;
        }

        if (!isDoubleType(pvApiCtx, piAddressVarThree))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 3);
            return 0;
        }

        if (getScalarDouble(pvApiCtx, piAddressVarThree, &dValueThree) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        iValueThree = (int)dValueThree;
        if ((double) iValueThree != dValueThree)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 3);
            return 0;
        }

        if ( (iValueThree < 1) && (iValueThree != -1) )
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A value > 0 expected.\n"), fname, 3);
            return 0;
        }


    }

    if (Rhs > 1) /* Rhs >=  2 */
    {
        int *piAddressVarTwo = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (isDoubleType(pvApiCtx, piAddressVarTwo))
        {
            if (Lhs == 2)
            {
                Scierror(78, _("%s: Wrong number of output arguments: %d expected.\n"), fname, 1);
                return 0;
            }

            if (isEmptyMatrix(pvApiCtx, piAddressVarTwo))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                return 0;
            }

            if ( (isVector(pvApiCtx, piAddressVarTwo)) ||
                    (isRowVector(pvApiCtx, piAddressVarTwo)) ||
                    (isScalar(pvApiCtx, piAddressVarTwo)) )
            {
                double *pdVarTwo = NULL;
                int m2 = 0;
                int n2 = 0;

                sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &pdVarTwo);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                    return 0;
                }

                if (getAllocatedSingleWideString(pvApiCtx, piAddressVarOne, &pStVarOne) != 0)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }
                else
                {
                    int m_out = 0, n_out = 0;
                    strsplit_error ierr = STRSPLIT_NO_ERROR;
                    wchar_t **results = strsplitfunction(pStVarOne, pdVarTwo, m2 * n2, &ierr);

                    if (pStVarOne)
                    {
                        freeAllocatedSingleWideString(pStVarOne);
                        pStVarOne = NULL;
                    }

                    switch (ierr)
                    {
                        case STRSPLIT_NO_ERROR:
                        {
                            m_out = (m2 * n2) + 1;
                            n_out = 1;

                            sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 1, m_out, n_out, results);

                            freeArrayOfWideString(results, m_out);
                            results = NULL;

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
                        break;
                        case STRSPLIT_INCORRECT_VALUE_ERROR:
                        {
                            Scierror(116, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                            return 0;
                        }
                        break;
                        case STRSPLIT_INCORRECT_ORDER_ERROR:
                        {
                            Scierror(99, _("%s: Elements of %dth argument must be in increasing order.\n"), fname, 2);
                            return 0;
                        }
                        break;
                        case STRSPLIT_MEMORY_ALLOCATION_ERROR:
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                            return 0;
                        }
                        break;
                        default:
                        {
                            Scierror(999, _("%s: error.\n"), fname);
                            return 0;
                        }
                        break;
                    }
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 2);
                return 0;
            }
        }
        else if (isStringType(pvApiCtx, piAddressVarTwo))
        {
            if (!isScalar(pvApiCtx, piAddressVarTwo))
            {
                wchar_t **pStrsTwo = NULL;
                int m2 = 0;
                int n2 = 0;

                if (getAllocatedMatrixOfWideString(pvApiCtx, piAddressVarTwo, &m2, &n2, &pStrsTwo) != 0)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }
                else
                {
                    if (pStrsTwo)
                    {
                        int k = 0;

                        /* checks that 2nd parameter is not an array of regexp pattern */
                        for (k = 0; k < m2 * n2; k++)
                        {
                            if (isValidRegExp(pStrsTwo[k]))
                            {
                                freeAllocatedMatrixOfWideString(m2, n2, pStrsTwo);
                                pStrsTwo = NULL;

                                Scierror(999, _("%s: Wrong value for input argument #%d: a string expected, not a regexp pattern.\n"), fname, 2);
                                return 0;
                            }
                        }
                        freeAllocatedMatrixOfWideString(m2, n2, pStrsTwo);
                        pStrsTwo = NULL;
                    }
                }
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 2);
            return 0;
        }
    }

    // we call %_strsplit macro
    C2F(overload)(&lw, fname, fname_len);

    return 0;
}
/*----------------------------------------------------------------------------*/
static BOOL isValidRegExp(wchar_t *pStr)
{
    if (pStr)
    {
        int lenStr = (int)wcslen(pStr);
        if (lenStr > 2)
        {
            return (BOOL) ((pStr[0] == L'/') && (pStr[lenStr - 1] == L'/'));
        }
    }
    return FALSE;
}
/*----------------------------------------------------------------------------*/
