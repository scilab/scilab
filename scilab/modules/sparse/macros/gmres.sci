// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


// [x, flag, resNorm, iter, resVec] = gmres( A, b, x, M, restrt, max_it, tol )
//
// GMRES solves the linear system Ax=b
// using the Generalized Minimal RESidual ( GMRES ) method with restarts .
//
// input   A        REAL nonsymmetric positive definite matrix or function
//         x        REAL initial guess vector
//         b        REAL right hand side vector
//         M        REAL preconditioner matrix or function
//         restrt   INTEGER number of iterations between restarts
//         max_it   INTEGER maximum number of iterations
//         tol      REAL error tolerance
//
// output  x        REAL solution vector
//         flag     INTEGER: 0 = solution found to tolerance
//                           1 = no convergence given max_it
//         resNorm      REAL final residual norm
//         iter     INTEGER number of iterations performed
//         resVec      REAL residual vector

//     Details of this algorithm are described in
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

function [x, flag, resNorm, iter, resVec] = gmres(A, varargin)

    // -----------------------
    // Parsing input arguments
    // -----------------------

    [lhs,rhs]=argn(0);
    if ( rhs < 2 ),
        error(msprintf(gettext("%s: Wrong number of input argument: At least %d expected.\n"),"gmres",2));
    end

    // Parsing the matrix A et the right hand side vector b
    select type(A)
    case 1 then
        matrixType = 1;
    case 5 then
        matrixType = 1;
    case 13 then
        matrixType = 0;
    end
    // If A is a matrix (full or sparse)
    if (matrixType == 1),
        if (size(A,1) ~= size(A,2)),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"gmres",1));
        end
    end
    b=varargin(1);
    if (size(b,2) ~= 1),
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Column vector expected.\n"),"gmres",2));
    end
    if (matrixType==1),
        if (size(b,1) ~= size(A,1)),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"gmres",2,1));
        end
    end

    // Number of iterations between restarts
    if (rhs >= 3),
        restrt=varargin(2);
        if (size(restrt) ~= [1 1]),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Scalar expected.\n"),"gmres",3));
        end
    else
        restrt=20;
    end

    // Error tolerance tol
    if (rhs >= 4),
        tol=varargin(3);
        if (size(tol) ~= [1 1]);
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Scalar expected.\n"),"gmres",4));
        end
    else
        tol = 1e-6;
    end

    // Maximum number of iterations max_it
    if (rhs >= 5),
        max_it=varargin(4);
        if (size(max_it) ~= [1 1]),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Scalar expected.\n"),"gmres",5));
        end
    else
        max_it=size(b,1);
    end

    // Parsing of the preconditioner matrix M
    if (rhs >= 6),
        M = varargin(5);
        select type(M)
        case 1 then
            precondType = 1;
        case 5 then
            precondType = 1;
        case 13 then
            precondType = 0;
        end
        if (precondType == 1),
            if (size(M,1) ~= size(M,2)),
                error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"gmres",4));
            end
            if (size(M,1) == 0),
                precondType = 2; // no preconditionning
            elseif ( size(M,1) ~= size(b,1) ),
                error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"gmres",4,2));
            end
        end
    else
        precondType = 2; // no preconditionning
    end

    // Parsing of the initial vector x
    if (rhs >= 7),
        x=varargin(6);
        if (size(x,2) ~= 1),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Column vector expected.\n"),"gmres",3));
        end
        if ( size(x,1) ~= size(b,1) ),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"gmres",3,2));
        end
    else
        x=zeros(b);
    end

    if (rhs > 7),
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"gmres",2,7));
    end

    // ------------
    // Computations
    // ------------

    j = 0;
    flag = 0;
    it2 = 0;

    bnrm2 = norm(b);
    if (bnrm2 == 0.0),
        x = zeros(b);
        resNorm = 0;
        iter = 0;
        resVec = resNorm;
        flag = 0;
        return
    end

    // r = M \ ( b-A*x );
    if (matrixType == 1),
        r = b - A*x;
    else
        r = b - A(x);
    end
    if (precondType == 1),
        r = M \ r;
    elseif (precondType == 0),
        r = M(r);
    end
    resNorm = norm(r)/bnrm2;
    resVec = resNorm;
    if (resNorm < tol),
        iter=0;
        return;
    end

    n = size(b,1);
    m = restrt;
    V(1:n,1:m+1) = zeros(n,m+1);
    H(1:m+1,1:m) = zeros(m+1,m);
    cs(1:m) = zeros(m,1);
    sn(1:m) = zeros(m,1);
    e1    = zeros(n,1);
    e1(1) = 1.0;

    for j = 1:max_it
        // r = M \ ( b-A*x );
        if (matrixType == 1),
            r = b - A*x;
        else
            r = b - A(x);
        end
        if (precondType == 1),
            r = M \ r;
        elseif (precondType == 0),
            r = M(r);
        end

        V(:,1) = r / norm( r );
        s = norm( r )*e1;
        for i = 1:m      // construct orthonormal
            it2 = it2 + 1; // basis using Gram-Schmidt
            // w = M \ (A*V(:,i));
            if (matrixType == 1),
                w = A*V(:,i);
            else
                w = A(V(:,i));
            end
            if (precondType == 1),
                w = M \ w;
            elseif (precondType == 0),
                w = M(w);
            end

            for k = 1:i
                H(k,i)= w'*V(:,k);
                w = w - H(k,i)*V(:,k);
            end
            H(i+1,i) = norm( w );
            V(:,i+1) = w / H(i+1,i);
            for k = 1:i-1 // apply Givens rotation
                temp     =  cs(k)*H(k,i) + sn(k)*H(k+1,i);
                H(k+1,i) = -sn(k)*H(k,i) + cs(k)*H(k+1,i);
                H(k,i)   = temp;
            end
            // form i-th rotation matrix
            [tp1,tp2] = rotmat( H(i,i), H(i+1,i) );
            cs(i)  = tp1;
            sn(i)  = tp2;
            temp   = cs(i)*s(i);
            s(i+1) = -sn(i)*s(i);
            s(i)   = temp;
            H(i,i) = cs(i)*H(i,i) + sn(i)*H(i+1,i);
            H(i+1,i) = 0.0;
            resNorm  = real(abs(s(i+1))) / bnrm2;
            resVec = [resVec;resNorm];
            if ( resNorm <= tol ),
                y = H(1:i,1:i) \ s(1:i);
                x = x + V(:,1:i)*y;
                break;
            end
        end
        if (resNorm <= tol),
            iter = j-1+it2;
            break;
        end
        y = H(1:m,1:m) \ s(1:m);
        // update approximation
        x = x + V(:,1:m)*y;
        // r = M \ ( b-A*x )
        if (matrixType == 1),
            r = b - A*x;
        else
            r = b - A(x);
        end
        if (precondType == 1),
            r = M \ r;
        elseif (precondType == 0),
            r = M(r);
        end
        s(j+1) = norm(r);
        resNorm = real(s(j+1)) / bnrm2;
        resVec = [resVec; resNorm];

        if ( resNorm <= tol ),
            iter = j+it2;
            break;
        end
        if ( j== max_it ),
            iter=j+it2;
        end
    end
    if ( resNorm > tol ),
        flag = 1;
        if (lhs < 2),
            warning(msprintf(gettext("%s: Did not converge.\n"),"gmres"));
        end
    end
endfunction //GMRES


//
// Compute the Givens rotation matrix parameters for a and b.
//
function [ c, s ] = rotmat( a, b )
    if ( b == 0.0 ),
        c = 1.0;
        s = 0.0;
    elseif ( abs(b) > abs(a) ),
        temp = a / b;
        s = 1.0 / sqrt( 1.0 + temp^2 );
        c = temp * s;
    else
        temp = b / a;
        c = 1.0 / sqrt( 1.0 + temp^2 );
        s = temp * c;
    end
endfunction //rotmat
