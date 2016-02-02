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
// bicg --
//   BICG solves the linear system %Ax=b using the BiConjugate Gradient method.
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
//     "Numerical Recipes: The Art of Scientific Computing." (third ed.)
//     by William Press, Saul Teukolsky, William Vetterling, Brian Flannery.
//
//     http://apps.nrbook.com/empanel/index.html?pg=87
//     http://dl.acm.org/citation.cfm?doid=1874391.187410
//     http://mathworld.wolfram.com/BiconjugateGradientMethod.html
//
// Notes
//     This script was originally a matlab > scilab translation of the bicg.m
//     script from http://www.netlib.org/templates/matlab
//
//     The input / output arguments of this command are the same as Matlab's bicg command.
//

function [x, resNorm, iter, resVec] = %bicg(%A, %b, tol, maxIter, %M, %M2, x0, verbose )

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
        // Solve M M2 r = r
        if %M == [] & %M2 == [] then
            z = r;
        elseif %M2 == [] then
            // Compute r so that M r = r
            if (precondType == 1) then
                z = %M \ r;
            elseif (precondType == 2) then
                z = %M(r,Margs(:));
            else
                z = r;
            end
        else
            // Compute r so that M M2 r = r
            if (precondBis == 1) then
                z = %M \ r;
                z = %M2 \ r;
            elseif (precondBis == 2) then
                z = %M(r,Margs(:));
                z = %M2(r,M2args(:));
            else
                z = r;
            end
        end
        // Solve M' M2' r2 = r2
        if %M == [] & %M2 == [] then
            z2 = r2;
        elseif %M2 == [] then
            // Compute r so that M' r = r
            if (precondType == 1) then
                z2 = %M' \ r2;
            elseif (precondType == 2) then
                z2 = %M(r2,Margs(:));
            else
                z2 = r2;
            end
        else
            // Compute r so that M' M2' r = r
            if (precondBis == 1) then
                z2 = %M' \ r2;
                z2 = %M2' \ r2;
            elseif (precondBis == 2) then
                z2 = %M(r2,Margs(:));
                z2 = %M2(r2,M2args(:));
            else
                z2 = r2;
            end
        end
        rho = r'*z2;
        if (rho == 0) then
            break;
        end
        if (currentiter > 1) then
            bet = rho / rho_old;
            p   = z + bet*p;
            p2  = z2 + bet*p2;
        else
            p  = z;
            p2 = z2;
        end
        // q = %A*p; q2 = %A'*p2;
        if (matrixType == 1),
            q  = %A*p;
            q2 = %A'*p2;
        else
            q  = %A(p);
            q2 = %A(p2);
        end
        alp = rho / (p2'*q);
        x   = x + alp*p;
        r   = r - alp*q;
        r2  = r2 - alp*q2;
        resNorm = norm(r) / bnrm2;
        // Caution : transform the scalar resVec into vector resVec !
        resVec = [resVec;resNorm];
        rho_old = rho;
    end
    // Test for convergence
    if (resNorm > tol) then
        if (verbose==1) then
            printf(gettext("Final residual = %s > tol =%s\n"),string(resNorm),string(tol));
            printf(gettext("Algorithm fails\n"));
        end
        flag = 1;
        if (lhs < 2) then
            warning(msprintf(gettext("%s: Convergence error.\n"),"bicg"));
        end
    else
        flag = 0;
        if (verbose==1) then
            printf(gettext("Algorithm pass\n"));
        end
    end

endfunction
