// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2006 - INRIA - Serge Steer
// Copyright (C) 2005 - IRISA - Sage Group
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//
// pcg --
//   PCG solves the symmetric positive definite linear system %Ax=b
//   using the Preconditionned Conjugate Gradient.
//   If M is given, it is used as a preconditionning matrix.
//   If both M and M2 are given, the matrix M * M2 is used as a preconditionning
//   matrix.
//
// input   %A       REAL symmetric positive definite matrix or a function
//                  y=Ax(x) which computes  y=%A*x for a given x
//         b        REAL right hand side vector
//         tol, optional      REAL error tolerance (default: 1e-8)
//         maxIter, optional  INTEGER maximum number of iterations (default: size(%b))
//         %M, optional       REAL preconditioner matrix (default: none)
//         %M2, optional      REAL preconditioner matrix (default: none)
//         x0, optional       REAL initial guess vector (default: the zero vector)
//         verbose, optional  INTEGER set to 1 to enable verbose logging (default : 0)
//
// output  x        REAL solution vector
//         resNorm  REAL final relative norm of the residual
//         iter     INTEGER number of iterations performed
//         resVec   REAL residual vector
//
// References
//
//     "Templates for the Solution of Linear Systems: Building Blocks
//     for Iterative Methods",
//     Barrett, Berry, Chan, Demmel, Donato, Dongarra, Eijkhout,
//     Pozo, Romine, and Van der Vorst, SIAM Publications, 1993
//     (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps).
//
//     "Iterative Methods for Sparse Linear Systems, Second Edition"
//     Saad, SIAM Publications, 2003
//     (ftp ftp.cs.umn.edu; cd dept/users/saad/PS; get all_ps.zip).
//
//     Golub and Van Loan, Matrix Computations
//
// Notes
//     This script was originally a matlab > scilab translation of the cg.m
//     script from http://www.netlib.org/templates/matlab
//
//     The input / output arguments of this command are the same as
//     Matlab's cg command.
//

function [x, resNorm, iter, resVec] = %pcg(%A, %b, tol, maxIter, %M, %M2, x0, verbose )

    // Initialization
    bnrm2 = norm(%b);
    if (verbose==1) then
        printf(gettext("Norm of right-hand side : %s\n"), string(bnrm2));
    end
    if  (bnrm2 == 0) then
        if (verbose==1) then
            printf(gettext("Special processing where the right-hand side is zero.\n"));
        end
        // When rhs is 0, there is a trivial solution : x=0
        x = zeros(%b);
        resNorm = 0;
        resVec = resNorm;
    else
        x = x0;
        // r = %b - %A*x;
        if (matrixType ==1),
            r = %b - %A*x;
        else
            r = %b - %A(x,Aargs(:));
        end
        resNorm = norm(r) / bnrm2;
        resVec = resNorm;
    end
    if (verbose==1) then
        printf(gettext("  Type of preconditionning #1 : %d\n"),precondType);
        printf(gettext("  Type of preconditionning #2 : %d\n"),precondBis);
    end
    // Begin iteration
    // Distinguish the number of iterations processed from the currentiter index
    iter = 0
    for currentiter = 1:maxIter
        if (resNorm <= tol) then
            if (verbose==1) then
                printf(gettext("  New residual = %s < tol = %s => break\n"),string(resNorm),string(tol));
            end
            break;
        end
        iter = iter + 1
        if (verbose==1) then
            printf(gettext("  Iteration #%s/%s residual : %s\n"),string(currentiter),string(maxIter),string(resNorm));
            printf("  x=\n");
            disp(x);
        end
        if %M == [] & %M2 == [] then
            z = r;
        elseif %M2 == [] then
            // Compute z so that M z = r
            if (precondType == 1) then
                z = %M \ r;
            elseif (precondType == 2) then
                z = %M(r,Margs(:));
            else
                z = r;
            end
        else
            // Compute z so that M M2 z = r
            if (precondBis == 1) then
                z = %M \ r;
                z = %M2 \ z;
            elseif (precondBis == 2) then
                z = %M(r,Margs(:));
                z = %M2(z,M2args(:));
            else
                z = r;
            end
        end
        rho = r'*z;
        if (currentiter > 1) then
            bet = rho / rho_old;
            p = z + bet*p;
        else
            p = z;
        end
        // q = %A*p;
        if (matrixType ==1),
            q = %A*p;
        else
            q = %A(p);
        end
        alp = rho / (p'*q );
        x = x + alp*p;
        r = r - alp*q;
        resNorm = norm(r) / bnrm2;
        // Caution : transform the scalar resVec into vector resVec !
        resVec = [resVec;resNorm];
        rho_old = rho;
    end

endfunction
