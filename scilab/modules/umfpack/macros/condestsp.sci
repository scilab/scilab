//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This set of scilab 's macros provide a few sparse utilities.
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [K1] = condestsp(A, arg2, arg3)
    //                     (A, LUp  , t)
    //  PURPOSE
    //     Give an estimate of the 1-norm condition number of
    //     the sparse matrix A by Algorithm 2.4 appearing in :
    //
    //      "A block algorithm for matrix 1-norm estimation
    //       with an application to 1-norm pseudospectra"
    //       Nicholas J. Higham and Francoise Tisseur
    //       Siam J. Matrix Anal. Appl., vol 21, No 4, pp 1185-1201
    //
    //  PARAMETERS
    //     A   : a square sparse matrix
    //
    //     LUp : (optional) a pointer to (umf) LU factors of A
    //           gotten by a call to umf_lufact ; if you
    //           have already computed the LU (= PAQ) factors
    //           it is recommanded to give this optional
    //           parameter (as the factorization may be time
    //           consuming)
    //
    //     t   : (optional) a positive integer
    //
    //     K1  : estimated 1-norm condition number of A
    //
    //  POSSIBLE SYNTAXES
    //     [K1, [x]] = condestsp(A, LUp, t)
    //     [K1, [x]] = condestsp(A, LUp)
    //     [K1, [x]] = condestsp(A, t)
    //     [K1, [x]] = condestsp(A)
    //
    //  AUTHOR
    //     Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> but nearly
    //     close to the given algorithm as this one is written in
    //     a "matlab-like" language
    //
    [lhs, rhs] = argn()

    if rhs<1 | rhs>3 | lhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"condestsp",1,3))
    end

    if typeof(A) ~= "sparse"  then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Square sparse matrix expected.\n"),"condestsp",1))
    else
        [n,m] = size(A)
        if n~=m  then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Square sparse matrix expected.\n"),"condestsp",1))
        end
    end

    factor_inside = %f  // when LUp is given (after the following tests
    // this var is set to %t if the factorization
    // is computed inside this function)
    if rhs == 1 then
        LUp = umf_lufact(A) ; factor_inside = %t ; t = 2
    elseif rhs == 2 then
        if typeof(arg2) == "pointer" then
            LUp = arg2 ; t = 2
        else
            t = arg2 ; LUp = umf_lufact(A) ; factor_inside = %t
        end
    elseif rhs == 3 then
        LUp = arg2 ; t = arg3
    end

    // verify if LUp and T are valid !
    [OK, nrow, ncol] = umf_luinfo(LUp)
    if ~OK then
        error(" the LU pointer is not valid")
    elseif n ~= nrow | nrow ~= ncol
        error(msprintf(gettext("%s: The matrix and the LU factors have not the same dimension !\n"),"condestsp"));
    end

    if int(t)~=t | length(t)~=1 | or(t < 1) then
        error(msprintf(gettext("%s: Invalid type and/or size and/or value for the second arg.\n"),"condestsp"));
    end

    // go on

    // the algo need a fortran-like sign function (with sign(0) = 1
    // and not with sign(0)=0 as the scilab native 's one)
    deff("s = fsign(x)", "s = sign(x) ; s(find(s == 0)) = 1")


    // Part 1 : computes ||A||_1
    norm1_A = norm(A,1)

    // Part 2 : computes (estimates) || A^(-1) ||_1

    // 1/ choose starting matrix X (n,t)
    X = ones(n,t)
    rand("seed", 0);
    X(:,2:t) = fsign(rand(n,t-1)-0.5)
    X = test_on_columns(X) / n
    Y = zeros(X) ; Z = zeros(X)
    ind_hist = []
    est_old = 0
    ind = zeros(n,1)
    S = zeros(n,t)
    k = 1 ; itmax = 5

    while %t
        // solve Y = A^(-1) X <=> A Y = X
        for j=1:t
            Y(:,j) = umf_lusolve(LUp, X(:,j))
        end
        [est, ind_est] = max( sum(abs(Y),"r") )
        if est > est_old  |  k==2 then
            ind_best = ind_est
            w = Y(:,ind_best)
        end

        if k >= 2  &  est <= est_old then, est = est_old, break, end
        est_old = est ; S_old = S

        if k > itmax then , break , end

        S = fsign(Y)

        // if every column of S is // to a column of S_old then it is finish
        if and( abs(S_old'*S) == n ) then , break , end

        if t > 1 then
            // s'assurer qu'aucune colonne de S n'est // a une autre
            // ou a une colonne de S_old en remplacant des colonnes par rand{-1,1}
            S = test_on_columns(S,S_old)
        end

        // calcul de Z = A' S
        for j=1:t
            Z(:,j) = umf_lusolve(LUp, S(:,j),"A''x=b")
        end

        [h,ind] = gsort(max(abs(Z),"c"))
        if k >= 2  then
            if h(1) == h(ind_best) then , break , end
        end

        if (t > 1) & (k > 1) then
            j = 1
            for l=1:t
                while %t & (j<=length(ind))
                    if find(ind_hist == ind(j)) == [] then
                        ind(l) = ind(j)
                        j = j + 1
                        break
                    else
                        j = j + 1
                    end
                end
            end
        end
        X = zeros(n,t)
        for l = 1:t
            X(ind(l),l) = 1
        end
        ind_hist = [ind_hist ; ind(1:t)]
        k = k + 1
    end

    K1 = est * norm1_A
    if factor_inside then
        umf_ludel(LUp)
    end

endfunction
