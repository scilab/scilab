/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2010 - Allan CORNET
*
* This file is released under the 3-clause BSD license. See COPYING-BSD.
*/
/* -------------------------------------------------------------------------- */
/* WARNING:this gateway uses old scilab api and it does not work with Scilab 6*/
/* see help api_scilab for more information. */
/* -------------------------------------------------------------------------- */
#include "sciprint.h"
#include "Scierror.h"
#include "stack-c.h"
#include "machine.h"
#include "core_math.h"
/* -------------------------------------------------------------------------- */
/* SUBROUTINE DSYEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK, INFO ) */
/* [w,U] = dsyev(A) */
/* w = dsyev(A) */
/* -------------------------------------------------------------------------- */
extern int C2F(dsyev)();
/* -------------------------------------------------------------------------- */
int intdsyev(char* fname)
{
    int M = 0, N = 0, lA = 0;
    int NLHS = 0;
    int un = 1;
    int WORK = 0, lWORK = 0, LWORKMIN = 0, LWORK = 0, INFO = 0;
    int LDA = 0, lw = 0;
    int A = 1, W = 2;

    static int minlhs = 1, minrhs = 1, maxlhs = 2, maxrhs = 1;
    CheckRhs(minrhs, maxrhs) ;
    CheckLhs(minlhs, maxlhs) ;

    /*--------------------A---------------------------*/
    GetRhsVar(A, MATRIX_OF_DOUBLE_DATATYPE, &M, &N, &lA);
    /*  To be done: Check A symmetric and Real*/
    NLHS = Lhs;
    switch ( NLHS )
    {

        case 2:
            /*  [w,U]=dsyev(A)     */
            CreateVar(W, MATRIX_OF_DOUBLE_DATATYPE, &N, &un, &lw);
            WORK = 3;
            LWORKMIN = Max(1, 3 * N - 1);
            LWORK = LWORKMIN;
            /* LWORK=C2F(maxvol)(&WORK, "d", 1L);   max memory currently available */
            if (LWORK < LWORKMIN)
            {
                sciprint("not enough memory (use stacksize)");
                SciError(9999);
            };
            CreateVar(3, MATRIX_OF_DOUBLE_DATATYPE, &LWORK, &un, &lWORK);
            LDA = Max(1, N);
            C2F(dsyev)("V", "L", &N, stk(lA), &LDA, stk(lw), stk(lWORK), &LWORK, &INFO);
            if (INFO != 0)
            {
                C2F(errorinfo)("dsyev ", &INFO, 5L);
            }
            LhsVar(1) = A;
            LhsVar(2) = W;
            return 0;
            break;

        case 1:
            /*   w=dsyev(A)    */
            CreateVar(W, MATRIX_OF_DOUBLE_DATATYPE, &N, &un, &lw);
            WORK = 3;
            LWORKMIN = Max(1, 3 * N - 1);
            LWORK = C2F(maxvol)(&WORK, "d", 1L); /* max memory currently available */
            if (LWORK < LWORKMIN)
            {
                sciprint("not enough memory (use stacksize)");
                SciError(9999);
            };
            CreateVar(3, MATRIX_OF_DOUBLE_DATATYPE, &LWORK, &un, &lWORK);
            LDA = Max(1, N);
            C2F(dsyev)("N", "L", &N, stk(lA), &LDA, stk(lw), stk(lWORK), &LWORK, &INFO);
            if (INFO != 0)
            {
                C2F(errorinfo)("dsyev ", &INFO, 5L);
            }
            LhsVar(1) = W;
            return 0;
            break;

        default:
            return 0;
    }
    return(0);
}
/* -------------------------------------------------------------------------- */
