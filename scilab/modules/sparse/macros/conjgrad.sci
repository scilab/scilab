// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier: transformed into a gateway to
//                                                         propose pcg, cgs, bicg and bicgstab.
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
// conjgrad --
//   This function regroups four methods from the "Conjugate Gradient family" to solve the linear system %Ax=b:
//     - PCG (Preconditioned Conjugate Gradient): A must be symmetric positive definite,
//     - CGS (preconditioned Conjugate Gradient Squared): A must be square,
//     - BICG (preconditioned BiConjugate Gradient): A must be square,
//     - BICGSTAB (preconditioned BiConjugate Gradient Stabilized): A must be square (default method).
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
//         flag     INTEGER: 0 = solution found to tolerance
//                           1 = no convergence given maxIter
//         resNorm  REAL final relative norm of the residual
//         iter     INTEGER number of iterations performed
//         resVec   REAL residual vector
//
// References
//
//   PCG
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
//   CGS
//     "CGS, A Fast Lanczos-Type Solver for Nonsymmetric Linear systems"
//     by Peter Sonneveld
//
//     http://epubs.siam.org/doi/abs/10.1137/0910004
//     http://dl.acm.org/citation.cfm?id=64888&preflayout=flat
//     http://mathworld.wolfram.com/ConjugateGradientSquaredMethod.html
//
//   BICG
//     "Numerical Recipes: The Art of Scientific Computing." (third ed.)
//     by William Press, Saul Teukolsky, William Vetterling, Brian Flannery.
//
//     http://apps.nrbook.com/empanel/index.html?pg=87
//     http://dl.acm.org/citation.cfm?doid=1874391.187410
//     http://mathworld.wolfram.com/BiconjugateGradientMethod.html
//
//   BICGSTAB
//     "Bi-CGSTAB: A Fast and Smoothly Converging Variant of Bi-CG for the Solution of Nonsymmetric Linear Systems"
//     by Henk van der Vorst.
//
//     http://epubs.siam.org/doi/abs/10.1137/0913035
//     http://dl.acm.org/citation.cfm?id=131916.131930&coll=DL&dl=GUIDE&CFID=372773884&CFTOKEN=56630250
//     http://mathworld.wolfram.com/BiconjugateGradientStabilizedMethod.html
//
// Notes
//
//     The input / output arguments of this command are the same as
//     Matlab's pcg, cgs, bicg and bicgstab commands, augmented with the 'method' argument
//

function [x, flag, resNorm, iter, resVec] = conjgrad(%A, %b, method, tol, maxIter, %M, %M2, x0, verbose )

    [lhs, rhs] = argn(0);

    if rhs < 2 | rhs >9 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"conjgrad",2,9));
    end

    if exists("method", "local") == 0 then
        method = "bicgstab";
    end
    if exists("tol", "local") == 0 then
        tol = 1e-8
    end
    if exists("maxIter", "local") == 0 then
        maxIter = size(%b, 1)
    end
    if exists("%M", "local") == 0 then
        %M = []
    end
    if exists("%M2", "local") == 0 then
        %M2 = []
    end
    if exists("x0", "local") == 0 then
        x0 = zeros(%b);
    end
    if exists("verbose", "local") == 0 then
        verbose = 0;
    end
    if verbose == 1 then
        printf(gettext("Arguments:\n"));
        printf("  tol = "+string(tol)+"\n");
        printf("  maxIter = "+string(maxIter)+"\n");
        printf("  M = \n")
        disp(%M)
        printf("  M2 = \n");
        disp(%M2)
        printf("  x0 = \n");
        disp(x0)
        printf("  verbose = "+string(verbose)+"\n");
    end
    // Compute matrixType
    select type(%A)
    case 1 then
        matrixType = 1;
    case 5 then
        matrixType = 1;
    case 13 then
        matrixType = 0;
        Aargs = list()
    case 15 then
        Aargs = list(%A(2:$))
        // Caution : modify the input argument %A !
        %A = %A(1);
        matrixType = 0;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"conjgrad",1));
    end
    // If %A is a matrix (dense or sparse)
    if matrixType == 1 then
        if size(%A,1) ~= size(%A,2) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"conjgrad",1));
        end
    end
    // Check right hand side %b
    if type(%b) ~= 1
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A matrix expected.\n"),"conjgrad",2));
    end
    if size(%b,2) ~= 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Column vector expected.\n"),"conjgrad",2));
    end
    if matrixType == 1 then
        if size(%b,1) ~= size(%A,1) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"conjgrad",2,1));
        end
    end
    if matrixType == 1 then
        if size(%b,1) ~= size(%A,1) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"conjgrad",2,1));
        end
    end
    // Check method
    if type(method) ~= 10 | size(method) ~= [1 1]
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single String expected.\n"),"conjgrad",3));
    elseif and(method ~= ["pcg" "cgs" "bicg" "bicgstab"]),
        error(msprintf(gettext("%s: Wrong value for input argument #%d: %s, %s, %s or %s expected.\n"),"conjgrad",3,"pcg","cgs","bicg","bicgstab"));
    end
    // Check type of the error tolerance tol
    if or(size(tol) ~= [1 1]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar expected.\n"),"conjgrad",4));
    end
    // Check the type of maximum number of iterations maxIter
    if or(size(maxIter) ~= [1 1]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar expected.\n"),"conjgrad",5));
    end
    // Compute precondType
    select type(%M)
    case 1 then
        // M is a matrix
        // precondType = 0 if the M is empty
        //             = 1 if the M is not empty
        precondType = bool2s(size(%M,"*")>=1);
    case 5 then
        precondType = 1;
    case 13 then
        Margs = list()
        precondType = 2;
    case 15 then
        Margs = list(%M(2:$))
        // Caution : modify the input argument %M !
        %M = %M(1);
        precondType = 2;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"conjgrad",6));
    end
    if precondType == 1 then
        if size(%M,1) ~= size(%M,2) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"conjgrad",6));
        end
        if size(%M,1) ~= size(%b,1) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"conjgrad",6,2));
        end
    end
    // Compute precondBis
    select type(%M2)
    case 1 then
        // M2 is a matrix
        // precondBis = 0 if the M2 is empty
        //            = 1 if the M2 is not empty
        precondBis = bool2s(size(%M2,"*")>=1);
    case 5 then
        precondBis = 1;
    case 13 then
        M2args = list()
        precondBis = 2;
    case 15 then
        M2args = list(%M2(2:$))
        // Caution : modify the input argument %M2 !
        %M2 = %M2(1);
        // Caution : modify precondType again !
        precondType = 2;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"conjgrad",7));
    end
    if precondBis == 1 then
        if size(%M2,1) ~= size(%M2,2) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"conjgrad",7));
        end
        if size(%M2,1) ~= size(%b,1) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"conjgrad",7,2));
        end
    end
    // Check size of the initial vector x0
    if size(x0,2) ~= 1 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Column vector expected.\n"),"conjgrad",8));
    end
    if size(x0,1) ~= size(%b,1) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"conjgrad",8,2));
    end

    // ------------
    // Computations
    // ------------
    select method
    case "pcg"
        [x, resNorm, iter, resVec] = %pcg(%A, %b, tol, maxIter, %M, %M2, x0, verbose )
    case "cgs"
        [x, resNorm, iter, resVec] = %cgs(%A, %b, tol, maxIter, %M, %M2, x0, verbose )
    case "bicg"
        [x, resNorm, iter, resVec] = %bicg(%A, %b, tol, maxIter, %M, %M2, x0, verbose )
    else // "bicgstab"
        [x, resNorm, iter, resVec] = %bicgstab(%A, %b, tol, maxIter, %M, %M2, x0, verbose )
    end

    // Test for convergence
    if resNorm > tol then
        if verbose == 1 then
            printf(gettext("Final residual = %s > tol =%s\n"),string(resNorm),string(tol));
            printf(gettext("Algorithm fails\n"));
        end
        flag = 1;
        if lhs < 2 then
            warning(msprintf(gettext("%s: Convergence error.\n"),"conjgrad"));
        end
    else
        flag = 0;
        if verbose == 1 then
            printf(gettext("Algorithm pass\n"));
        end
    end

endfunction
