/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA -
* Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
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
#include "gw_cacsd.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
/*--------------------------------------------------------------------------*/
extern int C2F(ab01od)();
extern int C2F(icopy)(int *, int *, int *, int *, int *);
/*--------------------------------------------------------------------------*/
int sci_contr(char* fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrptrA = NULL;
    double* ptrA = NULL;
    int* piAddrptrB = NULL;
    double* ptrB = NULL;
    int* piAddrptrTOL = NULL;
    double* ptrTOL = NULL;
    int* ptrNCONT = NULL;
    double* ptrU = NULL;
    int* ptrKSTAIR = NULL;
    double* ptrV = NULL;
    int* ptrIWORK = NULL;
    double* ptrDWORK = NULL;
    int* ptrJUNK = NULL;

    int mA = 0;
    int nA = 0;
    int mB = 0;
    int nB = 0;
    int A = 0;
    int B = 0;
    int U = 0;
    int KSTAIR = 0;
    int V = 0;
    int LDA = 0;
    int LDB = 0;
    int LDU = 0;
    int LDV = 0;
    int LDWORK = 0;
    int N = 0;
    int M = 0;
    int mtol = 0;
    int ntol = 0;
    int un = 0;
    int one = 0;
    int INFO = 0;
    int INDCON = 0;
    int NCONT = 0;
    char  *JOBU = NULL;
    char *JOBV = NULL;
    double theTOL = 0;;

    /*     [NCONT,U,KSTAIR,V,A,B]=ab01od(A,B,[TOL])   */

    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 1, 6);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrptrA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrptrB);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddrptrA) || !checkInputArgumentType(pvApiCtx, 1, sci_matrix))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
        return 0;
    }

    if (isVarComplex(pvApiCtx, piAddrptrB) || !checkInputArgumentType(pvApiCtx, 2, sci_matrix))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 2);
        return 0;
    }

    if (nbInputArgument(pvApiCtx) == 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrptrTOL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (isVarComplex(pvApiCtx, piAddrptrTOL) || !checkInputArgumentType(pvApiCtx, 3, sci_matrix))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 3);
            return 0;
        }
    }

    theTOL = nc_eps();
    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrptrA, &mA, &nA, &ptrA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
        return 1;
    }

    A = 1;      /*     A */
    N = mA;
    theTOL = 0.2 * sqrt(2 * theTOL) * N;

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrptrB, &mB, &nB, &ptrB);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 2);
        return 1;
    }

    B = 2;      /*     B */
    M = nB;

    if (nA != mB || mA != nA )
    {
        Scierror(999, _("%s: Wrong values for input arguments #%d and #%d.\n"), fname, 1, 2);
        return 0;
    }
    if (nbInputArgument(pvApiCtx) == 3)
    {
        /*    TOL is given:   ab01od(A,B,tol)   */
        // Retrieve a matrix of double at position 3.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrptrTOL, &mtol, &ntol, &ptrTOL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 3);
            return 1;
        }

        theTOL = *ptrTOL;  /*     TOL */
        if (theTOL > 1.0 || theTOL < 0.0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in [%d %d].\n"), fname, 3, 0, 1);
            return 0;
        }
    }

    /*     dimensions...    */
    LDA = Max(1, N);
    LDB = LDA;
    LDU = LDA;
    LDV = Max(1, M);
    LDWORK = Max(1, N * M + Max(N, M) + Max(N, 3 * M));

    /*     other parameters of AB01OD   */
    JOBU = "N";
    if (nbOutputArgument(pvApiCtx) >= 2)
    {
        JOBU = "I";
    }
    JOBV = "N";
    if (nbOutputArgument(pvApiCtx) >= 4)
    {
        JOBV = "I";
    }

    /*     creating NCONT,U,KSTAIR,V,IWORK,DWORK   */
    un = 1;
    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, nbInputArgument(pvApiCtx) + 1, un, un, &ptrNCONT);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    NCONT = nbInputArgument(pvApiCtx) + 1;
    sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, N, N, &ptrU);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    U = nbInputArgument(pvApiCtx) + 2;
    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, nbInputArgument(pvApiCtx) + 3, un, N, &ptrKSTAIR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    KSTAIR = nbInputArgument(pvApiCtx) + 3;
    sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 4, M, M, &ptrV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    V = nbInputArgument(pvApiCtx) + 4;
    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, nbInputArgument(pvApiCtx) + 5, un, M, &ptrIWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 6, un, LDWORK, &ptrDWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    C2F(ab01od)( "A", JOBU, JOBV, &N, &M, (ptrA), &LDA,
                 (ptrB), &LDB, (ptrU), &LDU, (ptrV), &LDV,
                 (int*)(ptrNCONT), &INDCON, (int*)(ptrKSTAIR), &theTOL,
                 (int*)(ptrIWORK), (ptrDWORK), &LDWORK, &INFO );
    if (INFO != 0)
    {
        Scierror(998, _("%s: internal error, info=%d.\n"), "ab01od", INFO);
        return 0;
    }

    if (nbOutputArgument(pvApiCtx) >= 3)
    {
        /*     resizing KSTAIR      */
        sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, nbInputArgument(pvApiCtx) + 7, un, INDCON, &ptrJUNK);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        KSTAIR = nbInputArgument(pvApiCtx) + 7;
        one = 1;
        C2F(icopy)(&INDCON, (int*)(ptrKSTAIR), &un, (int*)(ptrJUNK), &one);
    }
    /*     lhs variables: [NCONT,U,KSTAIR,V,A,B]=ab01od(A,B)   */
    AssignOutputVariable(pvApiCtx, 1) = NCONT;
    AssignOutputVariable(pvApiCtx, 2) = U;
    AssignOutputVariable(pvApiCtx, 3) = KSTAIR;
    AssignOutputVariable(pvApiCtx, 4) = V;
    AssignOutputVariable(pvApiCtx, 5) = A;
    AssignOutputVariable(pvApiCtx, 6) = B;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
