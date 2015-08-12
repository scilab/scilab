/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA -
* Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "stack-c.h"
#include "sci_contr.h"
#include "Scierror.h"
#include "core_math.h"
#include "localization.h"
#include "stack3.h"
/*--------------------------------------------------------------------------*/
extern double C2F(dlamch)(char *CMACH, unsigned long int);
extern int C2F(ab01od)();
/*--------------------------------------------------------------------------*/
int intab01od(char* fname)
{
    int mA = 0;
    int nA = 0;
    int ptrA = 0;
    int mB = 0;
    int nB = 0;
    int ptrB = 0;
    int A = 0;
    int B = 0;
    int U = 0;
    int KSTAIR = 0;
    int V = 0;
    int ptrIWORK = 0;
    int ptrU = 0;
    int ptrTOL = 0;
    int ptrKSTAIR = 0;
    int ptrV = 0;
    int ptrDWORK = 0;
    int ptrJUNK = 0;
    int ptrNCONT = 0;
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

    CheckRhs(2, 3);
    CheckLhs(1, 6);

    if (iIsComplex(1) || GetType(1) != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
        return 0;
    }

    if (iIsComplex(2) || GetType(2) != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 2);
        return 0;
    }

    if (Rhs == 3)
    {
        if (iIsComplex(3) || GetType(3) != sci_matrix)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 3);
            return 0;
        }
    }

    theTOL = (double) C2F(dlamch)("e", 1L);
    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &mA, &nA, &ptrA);
    A = 1;      /*     A */
    N = mA;
    theTOL = 0.2 * sqrt(2 * theTOL) * N;
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &mB, &nB, &ptrB);
    B = 2;      /*     B */
    M = nB;

    if (nA != mB || mA != nA )
    {
        Scierror(999, _("%s: Wrong values for input arguments #%d and #%d.\n"), fname, 1, 2);
        return 0;
    }
    if (Rhs == 3)
    {
        /*    TOL is given:   ab01od(A,B,tol)   */
        GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &mtol, &ntol, &ptrTOL);
        theTOL = *stk(ptrTOL);  /*     TOL */
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
    if (Lhs >= 2)
    {
        JOBU = "I";
    }
    JOBV = "N";
    if (Lhs >= 4)
    {
        JOBV = "I";
    }

    /*     creating NCONT,U,KSTAIR,V,IWORK,DWORK   */
    un = 1;
    CreateVar(Rhs + 1, MATRIX_OF_INTEGER_DATATYPE, &un, &un, &ptrNCONT);
    NCONT = Rhs + 1;
    CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &N, &N, &ptrU);
    U = Rhs + 2;
    CreateVar(Rhs + 3, MATRIX_OF_INTEGER_DATATYPE, &un, &N, &ptrKSTAIR);
    KSTAIR = Rhs + 3;
    CreateVar(Rhs + 4, MATRIX_OF_DOUBLE_DATATYPE, &M, &M, &ptrV);
    V = Rhs + 4;
    CreateVar(Rhs + 5, MATRIX_OF_INTEGER_DATATYPE, &un, &M, &ptrIWORK);
    CreateVar(Rhs + 6, MATRIX_OF_DOUBLE_DATATYPE, &un, &LDWORK, &ptrDWORK);
    C2F(ab01od)( "A", JOBU, JOBV, &N, &M, stk(ptrA), &LDA,
                 stk(ptrB), &LDB, stk(ptrU), &LDU, stk(ptrV), &LDV,
                 istk(ptrNCONT), &INDCON, istk(ptrKSTAIR), &theTOL,
                 istk(ptrIWORK), stk(ptrDWORK), &LDWORK, &INFO );
    if (INFO != 0)
    {
        C2F(errorinfo)("ab01od", &INFO, 6L);
        return 0;
    }
    if (Lhs >= 3)
    {
        /*     resizing KSTAIR      */
        CreateVar(Rhs + 7, MATRIX_OF_INTEGER_DATATYPE, &un, &INDCON, &ptrJUNK);
        KSTAIR = Rhs + 7;
        one = 1;
        C2F(icopy)(&INDCON, istk(ptrKSTAIR), &un, istk(ptrJUNK), &one);
    }
    /*     lhs variables: [NCONT,U,KSTAIR,V,A,B]=ab01od(A,B)   */
    LhsVar(1) = NCONT;
    LhsVar(2) = U;
    LhsVar(3) = KSTAIR;
    LhsVar(4) = V;
    LhsVar(5) = A;
    LhsVar(6) = B;
    return 0;
}
/*--------------------------------------------------------------------------*/
