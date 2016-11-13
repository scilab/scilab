/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - INRIA - Serge STEER
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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "core_math.h"
#include "gw_signal.h"
#include "sci_malloc.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "conv2.h"
/*--------------------------------------------------------------------------*/
int sci_conv2(char *fname, void *pvApiCtx)
{
    SciErr sciErr;
    int *piAddr = NULL;
    char *option = NULL;
    int iopt = 1;
    double *Outr = NULL, *Outi = NULL;
    int mOut = 0, nOut = 0;
    int iType = 0;
    int edgM = 0, edgN = 0;
    int rhs = Rhs;

    /* Check if last argument is one of the string "full", "same","valid" */
    sciErr = getVarAddressFromPosition(pvApiCtx, Rhs, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, Rhs);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr))
    {
        CheckRhs(3, 4);
        if (isScalar(pvApiCtx, piAddr))
        {
            if (getAllocatedSingleString(pvApiCtx, piAddr, &option) == 0)
            {
                if (strcmp("full", option) == 0)
                {
                    iopt = 1;
                }
                else if (strcmp("same", option) == 0)
                {
                    iopt = 2;
                }
                else if (strcmp("valid", option) == 0)
                {
                    iopt = 3;
                }
                else
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),
                             fname, Rhs, "\"full\", \"same\"", "\"valid\"");
                    freeAllocatedSingleString(option);
                    option = NULL;
                    return 0;
                }
                freeAllocatedSingleString(option);
                option = NULL;
                rhs = Rhs - 1;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),
                         fname, Rhs, "\"full\", \"same\"", "\"valid\"");
                return 0;
            }
        }
    }
    else
    {
        CheckRhs(2, 3);
    }

    if (rhs == 3)
    {
        /*separable conv2(C,R,A)*/
        double *Cr = NULL, *Ci = NULL;
        double *Rr = NULL, *Ri = NULL;
        double *Ar = NULL, *Ai = NULL;
        int mC = 0, nC = 0, mR = 0, nR = 0, mA = 0, nA = 0;

        /* get and check C */
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        //check type
        sciErr = getVarType(pvApiCtx, piAddr, &iType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (iType != sci_matrix)
        {
            Scierror(999, _("%s: Wrong type for argument #%d: Real vector expected.\n"),
                     fname, 1);
            return 0;
        }

        //get complexity
        if (isVarComplex(pvApiCtx, piAddr))
        {
            sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &mC, &nC, &Cr, &Ci);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

        }
        else
        {
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &mC, &nC, &Cr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

        }
        if (mC > 1 && nC > 1)
        {
            /*check if vector*/
            Scierror(999, _("%s: Wrong type for argument #%d: Real or complex vector expected.\n"),
                     fname, 1);
            return 0;
        }
        mC = mC * nC;

        /* get and check R */
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        //check type
        sciErr = getVarType(pvApiCtx, piAddr, &iType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (iType != sci_matrix)
        {
            Scierror(999, _("%s: Wrong type for argument #%d: Real or complex vector expected.\n"),
                     fname, 2);
            return 0;
        }

        //get complexity
        if (isVarComplex(pvApiCtx, piAddr))
        {
            sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &mR, &nR, &Rr, &Ri);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }

        }
        else
        {
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &mR, &nR, &Rr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }

        }

        if (mR > 1 && nR > 1)
        {
            /*check if vector*/
            Scierror(999, _("%s: Wrong type for argument #%d: Real vector expected.\n"),
                     fname, 2);
            return 0;
        }

        nR = nR * mR;
        /* get and check A */
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        //check type
        sciErr = getVarType(pvApiCtx, piAddr, &iType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (iType != sci_matrix)
        {
            Scierror(999, _("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"),
                     fname, 3);
            return 0;
        }

        //get complexity
        if (isVarComplex(pvApiCtx, piAddr))
        {
            sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &mA, &nA, &Ar, &Ai);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                return 0;
            }

        }
        else
        {
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &mA, &nA, &Ar);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                return 0;
            }

        }

        //Compute result sizes
        if (iopt == 1 )
        {
            if (mC == 0 || nR == 0)
            {
                mOut = mA;
                nOut = nA;
            }
            else
            {
                mOut = mA + mC - 1;
                nOut = nA + nR - 1;
            }
            edgM = mC - 1;
            edgN = nR - 1;
        }
        else if ( iopt == 2 )
        {
            mOut = mA;
            nOut = nA;
            edgM = ( mC - 1) / 2;
            edgN = ( nR - 1) / 2;
        }
        else if (iopt == 3)
        {
            if (mC == 0 || nR == 0)
            {
                mOut = mA;
                nOut = nA;
            }
            else
            {
                mOut = Max(0, mA - mC + 1);
                nOut = Max(0, nA - nR + 1);
            }
            edgM = edgN = 0;
        }

        if (Ri == NULL && Ci == NULL && Ai == NULL)
        {
            //real case
            double *Tr = NULL;
            //Allocate result
            sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, mOut, nOut, &Outr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 2, 1, nA, &Tr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            conv2_separable_R(Rr, nR, Cr, mC, Ar, mA, nA, Outr, mOut, nOut, edgM, edgN, Tr);
        }
        else
        {
            double *Tr = NULL, *Ti = NULL;
            //Allocate result
            sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, mOut, nOut, &Outr, &Outi);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 2, 1, nA, &Tr, &Ti);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            conv2_separable_C(Rr, Ri, nR, Cr, Ci, mC, Ar, Ai, mA, nA, Outr, Outi, mOut, nOut, edgM, edgN, Tr, Ti);
        }
    }
    else
    {
        /*conv2(A,B)*/
        double *Ar = NULL, *Ai = NULL;
        double *Br = NULL, *Bi = NULL;
        int mA, nA, mB, nB;

        /* get and check A */
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        //check type
        sciErr = getVarType(pvApiCtx, piAddr, &iType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (iType != sci_matrix)
        {
            Scierror(999, _("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"),
                     fname, 1);
            return 0;
        }

        //get complexity
        if (isVarComplex(pvApiCtx, piAddr))
        {
            sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &mA, &nA, &Ar, &Ai);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

        }
        else
        {
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &mA, &nA, &Ar);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

        }

        /* get and check B */
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        //check type
        sciErr = getVarType(pvApiCtx, piAddr, &iType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (iType != sci_matrix)
        {
            Scierror(999, _("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"),
                     fname, 2);
            return 0;
        }

        //get complexity
        if (isVarComplex(pvApiCtx, piAddr))
        {
            sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &mB, &nB, &Br, &Bi);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }

        }
        else
        {
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &mB, &nB, &Br);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }

        }

        if (iopt == 1)
        {
            if (mA == 0)
            {
                mOut = mB;
                nOut = nB;
            }
            else if (mB == 0)
            {
                mOut = mA;
                nOut = nA;
            }
            else
            {
                mOut = mA + Max(0, mB - 1);
                nOut = nA +  Max(0, nB - 1);
            }
            edgM = mB - 1;
            edgN = nB - 1;
        }
        else if (iopt == 2)
        {
            mOut = mA;
            nOut = nA;
            edgM = ( mB - 1) / 2;
            edgN = ( nB - 1) / 2;
        }
        else if (iopt == 3)
        {
            if (mB == 0)
            {
                mOut = mA;
                nOut = nA;
            }
            else
            {
                mOut = Max(0, mA - mB + 1);
                nOut = Max(0, nA - nB + 1);
            }
            edgM = edgN = 0;
        }

        if (Ai == NULL && Bi == NULL)
        {
            //real case
            //Allocate result
            sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, mOut, nOut, &Outr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            conv2_R(Ar, mA, nA, Br, mB, nB, Outr, mOut, nOut, edgM, edgN);
        }
        else
        {
            //Allocate result
            sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, mOut, nOut, &Outr, &Outi);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            conv2_C(Ar, Ai, mA, nA, Br, Bi, mB, nB, Outr, Outi, mOut, nOut, edgM, edgN);
        }
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
