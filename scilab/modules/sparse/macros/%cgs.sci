// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
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
// cgs --
//   CGS solves the linear system %Ax=b using the Conjugate Gradient Squared method.
//   If M is given, it is used as a preconditionning matrix.
//   If both M and M2 are given, the matrix M * M2 is used as a preconditionning
//   matrix.
//
// input   %A       REAL matrix or a function y=Ax(x) which computes y=%A*x for a given x
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
//     "CGS, A Fast Lanczos-Type Solver for Nonsymmetric Linear systems"
//     by Peter Sonneveld
//
//     http://epubs.siam.org/doi/abs/10.1137/0910004
//     http://dl.acm.org/citation.cfm?id=64888&preflayout=flat
//     http://mathworld.wolfram.com/ConjugateGradientSquaredMethod.html
//
// Notes
//     This script was originally a matlab > scilab translation of the cgs.m
//     script from http://www.netlib.org/templates/matlab
//
//     The input / output arguments of this command are the same as Matlab's cgs command.
//

function [x, resNorm, iter, resVec] = %cgs(%A, %b, tol, maxIter, %M, %M2, x0, verbose )

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
            r2 = r;
        else
            r = %b - %A(x,Aargs(:));
            r2 = r;
        end
        resNorm = norm(r) / bnrm2;
        resVec = resNorm;
    end
    if (verbose==1) then
        printf(gettext("  Type of preconditionning #1 : %d\n"),precondType);
        printf(gettext("  Type of preconditionning #2 : %d\n"),precondBis);
    end
    // begin iteration
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
        rho = r2'*r;
        if (rho == 0) then
            break;
        end
        if (currentiter > 1) then
            bet = rho / rho_old;
            u = r + bet*q;
            p = u + bet*(q+bet*p);
        else
            u = r;
            p = u;
        end
        // Solve M M2 P = p
        if %M == [] & %M2 == [] then
            P = p;
        elseif %M2 == [] then
            // Compute P so that M P = p
            if (precondType == 1) then
                P = %M \ p;
            elseif (precondType == 2) then
                P = %M(p,Margs(:));
            else
                P = p;
            end
        else
            // Compute P so that M M2 P = p
            if (precondBis == 1) then
                P = %M \ p;
                P = %M2 \ p;
            elseif (precondBis == 2) then
                P = %M(p,Margs(:));
                P = %M2(p,M2args(:));
            else
                P = p;
            end
        end
        // v = %A*P;
        if (matrixType ==1),
            v = %A*P;
        else
            v = %A(P);
        end
        alp = rho / (r2'*v);
        q = u - (alp*v);
        // Solve M M2 u = u+q
        uq = u + q;
        if %M == [] & %M2 == [] then
            U = uq;
        elseif %M2 == [] then
            // Compute Q so that M U = u+q
            if (precondType == 1) then
                U = %M \ uq;
            elseif (precondType == 2) then
                U = %M(uq,Margs(:));
            else
                U = uq;
            end
        else
            // Compute z so that M M2 U = u+q
            if (precondBis == 1) then
                U = %M \ uq;
                U = %M2 \ uq;
            elseif (precondBis == 2) then
                U = %M(uq,Margs(:));
                U = %M2(uq,M2args(:));
            else
                U = uq;
            end
        end
        x = x + alp*U;
        // U = %A*U;
        if (matrixType ==1),
            U = %A*U;
        else
            U = %A(U);
        end
        r = r - alp*U;
        resNorm = norm(r) / bnrm2;
        // Caution : transform the scalar resVec into vector resVec !
        resVec = [resVec;resNorm];
        rho_old = rho;
    end
    // test for convergence
    if (resNorm > tol) then
        if (verbose==1) then
            printf(gettext("Final residual = %s > tol =%s\n"),string(resNorm),string(tol));
            printf(gettext("Algorithm fails\n"));
        end
        flag = 1;
        if (lhs < 2) then
            warning(msprintf(gettext("%s: Convergence error.\n"),"cgs"));
        end
    else
        flag = 0;
        if (verbose==1) then
            printf(gettext("Algorithm pass\n"));
        end
    end

endfunction
