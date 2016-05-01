/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
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
// INTLINMEQ.F - Gateway function for solving Sylvester and Lyapunov matrix
//            equations using SLICOT routines SB04MD, SB04ND, SB04PD,
//            SB04QD, SB04RD, SB03MD, and SB03OD.
//
// Copyright INRIA 2001
// Scilab syntax
//   [X(,sep)] = linmeq(task,A(,B),C,flag,trans(,schur))
//
//   task = 1 :      [X] = linmeq(1,A,B,C,flag,trans,schur)
//   task = 2 :  [X,sep] = linmeq(2,A,C,flag,trans)
//                   [X] = linmeq(2,A,C,flag,trans)
//   task = 3 :      [X] = linmeq(3,A,C,flag,trans)
//
// Purpose:
//   To solve the Sylvester and Lyapunov linear matrix equations
//
//   task = 1:
//
//         op(A)*X + X*op(B) = C,                          (1a)
//
//         op(A)*X*op(B) + X = C,                          (1b)
//
//   task = 2:
//
//         op(A)'*X + X*op(A) = C,                         (2a)
//
//         op(A)'*X*op(A) - X = C,                         (2b)
//
//   task = 3:
//
//         op(A)'*(op(X)'*op(X)) + (op(X)'*op(X))*op(A) =
//                               -  op(//)'*op(//),          (3a)
//
//         op(A)'*(op(X)'*op(X))*op(A) - op(X)'*op(X) =
//                                     - op(//)'*op(//),     (3b)
//
//   where op(M) = M, if trans = 0, and op(M) = M', if trans = 1.
//
// Input parameters:
//   task  - integer option to determine the equation type:
//           = 1 : solve the Sylvester equation (1a) or (1b);
//           = 2 : solve the Lyapunov equation (2a) or (2b);
//           = 3 : solve for the Cholesky factor op(X) the Lyapunov
//                 equation (3a) or (3b).
//   A     - real coefficient N-by-N matrix.
//           When task = 3, matrix A must be stable.
//   B     - another real coefficient M-by-M matrix for
//           equations (1a) or (1b).
//   C     - right hand side matrix.
//           task = 1 : // is N-by-M;
//           task = 2 : // is N-by-N symmetric;
//           task = 3 : op(//) is P-by-N.
//   flag  - (optional) integer vector of length 3 or 2 containing
//           options.
//           task = 1 : flag has length 3
//                flag(1) = 0 : solve the continuous-time equation (1a);
//                              otherwise, solve the discrete-time
//                              equation (1b).
//                flag(2) = 1 : A is (quasi) upper triangular;
//                          2 : A is upper Hessenberg;
//                              otherwise, A is in general form.
//                flag(3) = 1 : B is (quasi) upper triangular;
//                          2 : B is upper Hessenberg;
//                              otherwise, B is in general form.
//           task = 2 : flag has length 2
//                flag(1) = 0 : solve continuous-time equation (2a);
//                              otherwise, solve discrete-time
//                              equation (2b).
//                flag(2) = 1 : A is (quasi) upper triangular;
//                              otherwise, A is in general form.
//           task = 3 : flag has length 2
//                flag(1) = 0 : solve continuous-time equation (3a);
//                              otherwise, solve discrete-time
//                              equation (3b).
//                flag(2) = 1 : A is (quasi) upper triangular;
//                              otherwise, A is in general form.
//           Default:    flag(1) = 0, flag(2) = 0 (, flag(3) = 0).
//   trans - (optional) integer specifying a transposition option.
//           trans = 0 : solve the equations (1) - (3) with op(M) = M.
//           trans = 1 : solve the equations (1) - (3) with op(M) = M'.
//           trans = 2 : solve the equations (1) with op(A) = A',
//                                                    op(B) = B.
//           trans = 3 : solve the equations (1) with op(A) = A,
//                                                    op(B) = B'.
//           Default:    trans = 0.
//   schur - (optional) integer specifying whether the Hessenberg-Schur
//           or Schur method should be used.
//           Available for task = 1.
//           schur = 1 : Hessenberg-Schur method (one matrix is reduced
//                       to Schur form).
//           schur = 2 : Schur method (two matrices are reduced to Schur
//                       form).
//           Default:    schur = 1.
//
// Output parameters:
//   X     - solution of the equation (or its Cholesky factor for (3)).
//   sep   - (optional) estimator of Sep(op(A),-op(A)') for (2.a) or
//           Sepd(A,A') for (2.b).
//
// Comments:
//   1. For equation (1a) or (1b), when schur = 1, the Hessenberg-Schur
//      method is used, reducing one matrix to Hessenberg form and the
//      other one to a real Schur form.
//      Otherwise, both matrices are reduced to real Schur forms.
//      If one or both matrices are already reduced to Schur/Hessenberg
//      forms, this could be specified by flag(2) and flag(3).
//      For general matrices, the Hessenberg-Schur method could be
//      significantly more efficient than the Schur method.
//   2. For equation (3a) or (3b), the computed matrix X is the Cholesky
//      factor of the solution, i.e., the real solution is op(X)'*op(X),
//      where X is an N-by-N upper triangular matrix.
//
// References:
//   This interface is based on the SLICOT routines:
//   SB04PD, SB04MD, SB04QD, DTRSYL, SB04PY, SB04ND, SB04RD, SB03MD, SB03OD
//
//
// Revisions:
//   Adapted from the Slicot Matlab Mexfile by S. Steer Oct 2001
//
// **********************************************************************
#include "gw_cacsd.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "elem_common.h"
#include "Sciwarning.h"

extern int C2F(lsame)();
extern int C2F(dlacpy)();
extern int C2F(dlaset)();
extern int C2F(dswap)();
extern int C2F(dtrsyl)();
extern int C2F(sb03md)();
extern int C2F(sb03od)();
extern int C2F(sb04md)();
extern int C2F(sb04nd)();
extern int C2F(sb04pd)();
extern int C2F(sb04py)();
extern int C2F(sb04qd)();
extern int C2F(sb04rd)();

int sci_linmeq(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrNA       = NULL;
    double* lA          = NULL;
    int* piAddrNB       = NULL;
    double* lB          = NULL;
    int* piAddrNC       = NULL;
    double* lC          = NULL;
    int* piAddrilTASK   = NULL;
    int* ilTASK         = NULL;
    int* piAddrilTRANS  = NULL;
    int* ilTRANS        = NULL;
    int* piAddrilSCHUR  = NULL;
    int* ilSCHUR        = NULL;
    int* piAddrilFLAG   = NULL;
    int* ilFLAG         = NULL;

    double* lDWORK  = NULL;
    int* lIWORK     = NULL;

    double* lU   = NULL;
    double* lV   = NULL;
    double* lWI  = NULL;
    double* lWR  = NULL;
    double* lCC  = NULL;
    double* lSEP = NULL;

    // .. Parameters ..
    double ONE  = 1.;
    double ZERO = 0.;
    int iOne    = 1;

    // .. Scalar parameters used by SLICOT subroutines ..
    char DICO = 0 , FACT = 0 , FACTA = 0 , FACTB = 0 , JOB = 0 , SCHU = 0 , TRANA = 0 , TRANB = 0 , ULA = 0 , ULB = 0;
    int INFO = 0, ISGN = 0, LDA = 0, LDB = 0, LDC = 0, LDU = 0, LDV = 0, NDWORK = 0, M = 0, N = 0, P = 0;
    double FERR = 0., SCALE = 0., SEP = 0., TOL = 0.;

    // .. Local variables and constant dimension arrays ..
    BOOL PERTRB = FALSE;
    int FLAG[3] = {0, 0, 0};
    int IB = 0, IP = 0, LDW1 = 0, LDW2 = 0, NIWORK = 0, MXMN = 0, NM = 0, NSCHUR = 0, TASK = 0, TRANS = 0;
    double TEMP = 0.;

    int Mt = 0, Nt = 0;
    int MA = 0, NA = 0;
    int MB = 0, NB = 0;
    int MC = 0, NC = 0;

    int i = 0;
    int j = 0;
    int iNbvars = Rhs;

    // Check for proper number of arguments.
    CheckInputArgument(pvApiCtx, 3, 7);
    CheckOutputArgument(pvApiCtx, 1, 2);

    // Check dimensions of input parameters and read/set scalar parameters.
    //   task
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrilTASK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrilTASK, &Mt, &Nt, &ilTASK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, IP);
        return 1;
    }

    if (Mt != 1 || Nt != 1)
    {
        Scierror(999, _("%s: %s must be a scalar.\n"), fname, "TASK");
        return 1;
    }

    TASK = *ilTASK;
    if (TASK < 1 || TASK > 3)
    {
        Scierror(999, _("%s: Wrong value for %s: %d, %d or %d expected.\n"), fname, "TRANS", 1, 2, 3);
        return 1;
    }

    if (TASK == 1)
    {
        if (nbInputArgument(pvApiCtx) < 4)
        {
            Scierror(999, _("%s: TASK = 1 : LINMEQ requires at least 4 input arguments.\n"), fname);
            return 1;
        }
        IP = 6;
    }
    else
    {
        IP = 5;
    }

    //   trans
    if (nbInputArgument(pvApiCtx) >= IP)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, IP, &piAddrilTRANS);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position IP.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrilTRANS, &Mt, &Nt, &ilTRANS);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, IP);
            return 1;
        }


        if (Mt != 1 || Nt != 1)
        {
            Scierror(999, _("%s: %s must be a scalar.\n"), fname, "TRANS");
            return 1;
        }

        TRANS = *ilTRANS;
    }

    if (TASK == 1 && (TRANS < 0 || TRANS > 3))
    {
        Scierror(999, _("%s: Wrong value for %s: %d, %d, %d or %d expected.\n"), fname, "TRANS", 0, 1, 2, 3);
        return 1;
    }
    else if (TASK != 1 && (TRANS < 0 || TRANS > 1))
    {
        Scierror(999, _("%s: Wrong value for %s: %d, or %d expected.\n"), fname, "TRANS", 0, 1);
        return 1;
    }

    //   schur
    if (TASK == 1)
    {
        NSCHUR = 1;
        if (nbInputArgument(pvApiCtx) == IP + 1)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, IP + 1, &piAddrilSCHUR);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position IP + 1.
            sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrilSCHUR, &Mt, &Nt, &ilSCHUR);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, IP + 1);
                return 1;
            }

            if (Mt != 1 || Nt != 1)
            {
                Scierror(999, _("%s: %s must be a scalar.\n"), fname, "SCHUR");
                return 1;
            }

            NSCHUR = *(int*)(ilSCHUR);

            if (NSCHUR < 1 || NSCHUR > 2)
            {
                Scierror(999, _("%s: Wrong value for %s: %d or %d expected.\n"), fname, "SCHUR", 1, 2);
                return 1;
            }
        }
    }

    //   A(NxN), (B(MxM),) C(NxM), or C(NxN), or op(C)(PxN)
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrNA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    if (isVarComplex(pvApiCtx, piAddrNA))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 2);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddrNA, &MA, &NA, &lA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"), fname, 2);
        return 1;
    }

    if (MA != NA)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), fname, 2);
        return 1;
    }

    N = MA;

    if (TASK == 1)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrNB);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 3.
        if (isVarComplex(pvApiCtx, piAddrNB))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 3);
            return 1;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddrNB, &MB, &NB, &lB);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"), fname, 3);
            return 1;
        }

        if (MB != NB)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), fname, 3);
            return 1;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrNC);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 4.
        if (isVarComplex(pvApiCtx, piAddrNC))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 4);
            return 1;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddrNC, &MC, &NC, &lC);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"), fname, 4);
            return 1;
        }

        if (MC != N)
        {
            Scierror(999, _("%s: Wrong column size for input argument #%d: Same column size of input argument #%d expected.\n"), fname, 4, 2);
            return 1;
        }

        M = NC;
        AssignOutputVariable(pvApiCtx, 1) = 4;
    }
    else
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrNC);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 3.
        if (isVarComplex(pvApiCtx, piAddrNC))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 3);
            return 1;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddrNC, &MC, &NC, &lC);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"), fname, 3);
            return 1;
        }

        //      if (TASK == 3){
        if (TRANS == 0)
        {
            P = MC;
            if (NC != N)
            {
                Scierror(999, _("%s: Wrong row size for input argument #%d: Same row size of input argument #%d expected.\n"), fname, 3, 2);
                return 1;
            }
        }
        else
        {
            if (MC != N)
            {
                Scierror(999, _("%s: Wrong row size for input argument #%d: Same row size of input argument #%d expected.\n"), fname, 3, 2);
                return 1;
            }
            P = NC;
        }
        //      }
        AssignOutputVariable(pvApiCtx, 1) = 3;
    }

    if (TASK == 1)
    {
        IP = 5;
    }
    else
    {
        IP = 4;
    }

    if (nbInputArgument(pvApiCtx) >= IP)
    {
        //   flag
        sciErr = getVarAddressFromPosition(pvApiCtx, IP, &piAddrilFLAG);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position IP.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrilFLAG, &Mt, &Nt, &ilFLAG);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, IP);
            return 1;
        }

        if (TASK == 1)
        {
            if (Mt * Nt > 3)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: At most 3 elements expected.\n"), fname, IP);
                return 1;
            }
        }
        else
        {
            if (Mt * Nt > 2)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: At most 2 elements expected.\n"), fname, IP);
                return 1;
            }
        }

        for (i = 0; i < Mt * Nt; i++)
        {
            FLAG[i] = ilFLAG[i];
        }
    }

    // Determine the lengths of working arrays.
    // Use a larger value for NDWORK for enabling calls of block algorithms
    // in DGEES, and possibly in DGEHRD, DGEQRF, DGERQF, SB04PD.
    LDA = Max(1, N);
    if (TASK == 1)
    {
        LDB = Max(1, M);
        if (NSCHUR == 2)
        {
            if (FLAG[1] == 1)
            {
                LDW1 = 0;
                LDW2 = 0;
            }
            else
            {
                LDW1 = 1 + 2 * N;
                LDW2 = 3 * N;
            }

            IB = 0;
            if (FLAG[2] != 1)
            {
                IB = 2 * M;
                if (FLAG[1] == 1)
                {
                    IB++;
                }
                LDW2 = Max(LDW2, IB + 3 * M);
            }
            LDW2 = Max(LDW2, IB + 2 * N);
            NDWORK = Max(1, LDW1 + LDW2);
        }
        else
        {
            if (FLAG[1] * FLAG[2] == 1)
            {
                NIWORK = 0;
                if (FLAG[0] != 0)
                {
                    NDWORK = 2 * N;
                }
                else
                {
                    NDWORK = 0;
                }
            }
            else if (FLAG[1] * FLAG[2] == 2)
            {
                MXMN = Max(M, N);
                NIWORK = 2 * MXMN;
                NDWORK = 2 * MXMN * (4 + 2 * MXMN);
            }
            else
            {
                NIWORK = 4 * N;
                NDWORK = Max(1, Max(5 * M, N + M));
                if (FLAG[0] == 0)
                {
                    NDWORK = Max(NDWORK, 2 * N * N + 8 * N);
                }
                else
                {
                    NDWORK = Max(NDWORK, 2 * N * N + 9 * N);
                }
            }
        }
    }
    else if (TASK == 2)
    {
        NDWORK = Max(1, Max(N * N , 3 * N));
        if (nbOutputArgument(pvApiCtx) == 2)
        {
            NDWORK = Max(NDWORK, 2 * N * N);
            if (FLAG[0] != 0)
            {
                NDWORK = Max(NDWORK, 2 * (N * N + N));
            }
        }
    }

    if (TASK != 3)
    {
        LDC = LDA;
    }
    else
    {
        if (TRANS == 0)
        {
            LDC = Max(1, Max(N, P));
        }
        else
        {
            LDC = LDA;
        }

        MXMN = Min(P, N);
        NDWORK = Max(1, 4 * N + MXMN);
    }

    // Allocate variable dimension local arrays.
    if (TASK == 1)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, NDWORK, iOne, &lDWORK);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        if (NSCHUR == 2)
        {
            if (FLAG[1] == 1)
            {
                FACTA = 'S';
                LDU = 1;

                sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, LDU, iOne, &lU);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }
            }
            else
            {
                FACTA = 'N';
                LDU = LDA;

                sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, LDU, N, &lU);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }
            }

            if (FLAG[2] == 1)
            {
                FACTB = 'S';
                LDV = 1;

                sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, LDV, iOne, &lV);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }
            }
            else
            {
                FACTB = 'N';
                LDV = LDB;
                sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, LDV, M, &lV);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }
            }
        }
        else
        {
            SCHU = 'N';
            sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, ++iNbvars, NIWORK, iOne, &lIWORK);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            if (FLAG[1] == 1)
            {
                if (FLAG[2] == 1)
                {
                    SCHU = 'S';
                }
                else if (FLAG[2] == 2)
                {
                    SCHU = 'A';
                }
            }
            else if (FLAG[1] == 2 && FLAG[2] == 1)
            {
                SCHU = 'B';
            }

            if (C2F(lsame)(&SCHU , "N"))
            {
                LDU = LDB;
                sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, LDU, M, &lU);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }
            }
        }
    }
    else if (TASK == 2)
    {
        LDU = LDA;
        sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, LDU, N, &lU);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, NDWORK, iOne, &lDWORK);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, N, iOne, &lWI);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, N, iOne, &lWR);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        if (nbOutputArgument(pvApiCtx) == 1)
        {
            sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, ++iNbvars, iOne, iOne, &lIWORK);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }
        }
        else
        {
            int NN = N * N;
            sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, ++iNbvars, NN, iOne, &lIWORK);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }
        }
    }
    else
    {
        LDU = LDA;
        sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, LDU, N, &lU);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, NDWORK, iOne, &lDWORK);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, N, iOne, &lWI);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, N, iOne, &lWR);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

    }

    // Do the actual computations.
    if (TASK == 1)
    {
        if (NSCHUR == 2)
        {
            if (TRANS == 0)
            {
                TRANA = 'N';
                TRANB = 'N';
            }
            else if (TRANS == 1)
            {
                TRANA = 'T';
                TRANB = 'T';
            }
            else if (TRANS == 2)
            {
                TRANA = 'T';
                TRANB = 'N';
            }
            else if (TRANS == 3)
            {
                TRANA = 'N';
                TRANB = 'T';
            }

            if (FLAG[0] != 0)
            {
                DICO = 'D';
            }
            else
            {
                DICO = 'C';
            }

            ISGN = 1;

            C2F(sb04pd)(&DICO, &FACTA, &FACTB, &TRANA, &TRANB, &ISGN, &N, &M,
                        lA, &LDA, lU, &LDU, lB, &LDB, lV, &LDV,
                        lC, &LDC, &SCALE, lDWORK, &NDWORK, &INFO);
        }
        else
        {
            if (TRANS == 0)
            {
                if (C2F(lsame)(&SCHU , "S"))
                {
                    TRANA = 'N';
                    TRANB = 'N';
                }
                else
                {
                    ULA = 'U';
                    ULB = 'U';
                }
            }
            else if (TRANS == 1)
            {
                if (C2F(lsame)(&SCHU , "S"))
                {
                    TRANA = 'T';
                    TRANB = 'T';
                }
                else
                {
                    ULA = 'L';
                    ULB = 'L';

                    for (j = 1; j < N; j++)
                    {
                        C2F(dswap)(&j, lA + j * LDA, &iOne, lA + j, &LDA);
                    }

                    for (j = 1; j < M; j++)
                    {
                        C2F(dswap)(&j, lB + j * LDB, &iOne, lB + j, &LDB);
                    }
                }
            }
            else if (TRANS == 2)
            {
                if (C2F(lsame)(&SCHU , "S"))
                {
                    TRANA = 'T';
                    TRANB = 'N';
                }
                else
                {
                    ULA = 'L';
                    ULB = 'U';
                    for (j = 1; j < N; j++)
                    {
                        C2F(dswap)(&j, lA + j * LDA, &iOne, lA + j, &LDA);
                    }
                }
            }
            else if (TRANS == 3)
            {
                if (C2F(lsame)(&SCHU , "S"))
                {
                    TRANA = 'N';
                    TRANB = 'T';
                }
                else
                {
                    ULA = 'U';
                    ULB = 'L';
                    for (j = 1; j < M; j++)
                    {
                        C2F(dswap)(&j, lB + j * LDB, &iOne, lB + j, &LDB);
                    }
                }
            }

            if (C2F(lsame)(&SCHU , "N"))
            {
                SCALE = ONE;
                if (FLAG[0] == 0)
                {
                    int i = 0;
                    C2F(sb04md)(&N, &M, lA, &LDA, lB, &LDB, lC,
                                &LDC, lU, &LDU, lIWORK, lDWORK, &NDWORK, &INFO);
                }
                else
                {
                    C2F(sb04qd)(&N, &M, lA, &LDA, lB, &LDB, lC,
                                &LDC , lU, &LDU, lIWORK, lDWORK, &NDWORK, &INFO);
                }
            }
            else if (C2F(lsame)(&SCHU , "S"))
            {
                if (FLAG[0] == 0)
                {
                    C2F(dtrsyl)(&TRANA, &TRANB, &iOne, &N, &M, lA, &LDA,
                                lB, &LDB, lC, &LDC, &SCALE, &INFO);
                }
                else
                {
                    C2F(sb04py)(&TRANA, &TRANB, &iOne, &N, &M, lA, &LDA,
                                lB, &LDB, lC, &LDC, &SCALE, lDWORK, &INFO);
                }
            }
            else
            {
                SCALE = ONE;
                TOL = ZERO;

                //              Default tolerance (epsilon_machine) is used.
                if (FLAG[0] == 0)
                {
                    C2F(sb04nd)(&SCHU, &ULA, &ULB, &N, &M, lA, &LDA,
                                lB, &LDB, lC, &LDC, &TOL, lIWORK,
                                lDWORK, &NDWORK, &INFO);
                }
                else
                {
                    C2F(sb04rd)(&SCHU, &ULA, &ULB, &N, &M, lA, &LDA,
                                lB, &LDB, lC, &LDC, &TOL, lIWORK,
                                lDWORK, &NDWORK, &INFO);
                }
            }
        }
    }
    else
    {
        if (FLAG[0] == 0)
        {
            DICO = 'C';
        }
        else
        {
            DICO = 'D';
        }

        if (FLAG[1] != 1)
        {
            FACT = 'N';
        }
        else
        {
            FACT = 'F';
            C2F(dlaset)("F", &N, &N, &ZERO, &ONE, lU, &LDU);
        }

        if (TRANS == 0)
        {
            TRANA = 'N';
        }
        else
        {
            TRANA = 'T';
        }

        if (TASK == 2)
        {
            if (nbOutputArgument(pvApiCtx) == 2)
            {
                JOB = 'B';
            }
            else
            {
                JOB = 'X';
            }

            if (MC < LDC)
            {
                int NMC = N * MC;
                sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, LDC, N, &lCC);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }


                C2F(dcopy)(&NMC, lC, &iOne, lCC, &iOne);
                C2F(sb03md)(&DICO, &JOB, &FACT, &TRANA, &N, lA, &LDA,
                            lU, &LDU, lCC, &LDC, &SCALE, &SEP, &FERR, lWR,
                            lWI, lIWORK, lDWORK, &NDWORK, &INFO);
                C2F(dcopy)(&NMC, lCC, &iOne, lC, &iOne);
            }
            else
            {
                C2F(sb03md)(&DICO, &JOB, &FACT, &TRANA, &N, lA, &LDA,
                            lU, &LDU, lC, &LDC, &SCALE, &SEP, &FERR, lWR,
                            lWI, lIWORK, lDWORK, &NDWORK, &INFO);
            }
        }
        else
        {
            C2F(sb03od)(&DICO, &FACT, &TRANA, &N, &P, lA, &LDA, lU,
                        &LDU, lC, &LDC, &SCALE, lWR, lWI, lDWORK,
                        &NDWORK, &INFO);
        }
    }

    // form output
    PERTRB =  (TASK == 1 && (INFO == N + M + 1 || (FLAG[1] * FLAG[2] == 1 && INFO == 1))) ||
              (TASK == 2 && INFO == N + 1) || (TASK == 3 && INFO == 1);

    if (INFO == 0 || PERTRB)
    {
        if (nbOutputArgument(pvApiCtx) >= 1)
        {
            if (TASK == 3)
            {
                if (TRANS == 0 && P > N)
                {
                    C2F(dlacpy)("U", &N, &N, lC, &LDC, lC, &LDA);
                }

                if (N > 1)
                {
                    int Nm1 = N - 1;
                    C2F(dlaset)("L", &Nm1, &Nm1, &ZERO, &ZERO, (lC + 1), &LDA);
                }
            }
        }

        if (TASK == 2)
        {
            if (nbOutputArgument(pvApiCtx) >= 2)
            {
                if (N == ZERO)
                {
                    SEP = ZERO;
                }

                sciErr = allocMatrixOfDouble(pvApiCtx, ++iNbvars, iOne, iOne, &lSEP);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }

                *lSEP = SEP;

                AssignOutputVariable(pvApiCtx, 2) = iNbvars;
            }
        }
    }

    // Error and warning handling.
    if (INFO > 0)
    {
        if (TASK == 1)
        {
            if (NSCHUR == 2)
            {
                //     .        messages from SB04PD
                if (INFO <= M + N)
                {
                    //     .           Failure when computing eigenvalues
                    Scierror(999, _("%s: Convergence problem...\n"), fname);
                }
                else if (INFO > M + N)
                {
                    //     .           Equation is singular
                    Scierror(999, _("%s: Problem is singular.\n"), fname);
                }
                return 1;
            }
            else
            {
                if (C2F(lsame)(&SCHU , "N"))
                {
                    //     .           messages from SB04MD or SB04QD
                    if (INFO <= M)
                    {
                        //     .              Failure when computing eigenvalues
                        Scierror(999, _("%s: Convergence problem...\n"), fname);
                    }
                    else if (INFO > M)
                    {
                        //     .              Equation is singular
                        Scierror(999, _("%s: Problem is singular.\n"), fname);
                    }
                }
                else
                {
                    //     .           Equation is singular
                    Scierror(999, _("%s: Problem is singular.\n"), fname);
                }
                return 1;
            }
        }
        else if (TASK == 2)
        {
            if (INFO <= N)
            {
                //     .        Failure when computing eigenvalues
                Scierror(999, _("%s: Convergence problem...\n"), fname);
            }
            else if (INFO > N)
            {
                //     .        Equation is singular
                Scierror(999, _("%s: Problem is singular.\n"), fname);
            }
            return 1;
        }
        else if (TASK == 3)
        {
            if (INFO == 1)
            {
                //     .        Equation is singular
                Scierror(999, _("%s: Problem is singular.\n"), fname);
            }
            else if (INFO == 2 || INFO == 3)
            {
                if (C2F(lsame)(&DICO, "C"))
                {
                    //     .        Matrix is not stable (cont)
                    Scierror(999, _("%s: Wrong value for input argument #%d: eigenvalues must have negative real parts.\n"), fname, 1);
                }
                else
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: eigenvalues modulus must be less than one.\n"), fname, 1);
                }
            }
            else if (INFO == 4 || INFO == 5)
            {
                //     .        not a schur form
                Scierror(999, _("%s: Wrong value for input argument #%d: eigenvalues modulus must be less than one.\n"), fname, 1);
            }
            else if (INFO == 6)
            {
                //     .        Failure when computing eigenvalues
                Scierror(999, _("%s: Convergence problem...\n"), fname);
            }
            return 1;
        }
    }
    else if (INFO < 0)
    {
        //     .  message displayed by slicot or lapack
        Scierror(999, " ");
    }

    if ((INFO == 0 || PERTRB) && SCALE != ONE)
    {
        if (TASK <= 2)
        {
            TEMP = SCALE;
        }
        else
        {
            TEMP = SCALE * SCALE;
        }
    }

    if (INFO != 0 && PERTRB == FALSE)
    {
        Scierror(999, _("%s: Warning: input arguments were scaled by %lf to avoid overflow.\n"), fname, TEMP);
        return 1;
    }
    else if (SCALE != ONE)
    {
        Sciwarning(_("%s: Warning: input arguments were scaled by %lf to avoid overflow.\n"), fname, TEMP);
    }

    if (PERTRB)
    {
        Sciwarning(_("%s: Warning: the equation is (almost) singular. Perturbed values have been used.\n"), fname);
    }

    ReturnArguments(pvApiCtx);
    return 0;
}
