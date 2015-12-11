// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel Gougeon <sgougeon@free.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function rep = %grand_perm(N, V)
    // Returns N random permutations of V's components.
    // V may be a scalar, a row or column vector, a matrix, or an hypermatrix.
    // All data types are supported: Integers, reals, complexes, strings, polynomes..
    // if V is a row, permutations are returned as N rows
    // if V is a column, permutations are returned as N columns
    // if V is a matrix or a true hypermatrix, permutations are returned
    // through an additional dimension of size N.

    // Author : Samuel Gougeon - 2010
    // Version: 1.2
    // Software: Scilab and related
    //
    // Tests:
    // with a row of reals:
    // m = grand(1, 6, "uin", 0, 10)
    // grand_perm(3, m)
    // // with a column of reals:
    // grand_perm(4, m')
    // // with a matrix of reals:
    // M = grand(2, 5, "uin", 0, 10)
    // grand_perm(3, M)
    // // with strings:
    // grand_perm(5, ["a" "b" "c" "d"])
    // // with polynomes:
    // p = (m'-%z)^2
    // grand_perm(3, p)

    s = size(V);
    L = prod(s);

    if L == 1 then
        rep = V(ones(1, N));
    else
        //[tmp, k] = gsort(rand(L, N), "r", "i");
        k = grand(N, "prm", (1:L).'); // Faster for large L
        if length(s) < 3 then
            if s(1) == 1 then
                rep = matrix(V(k), [s(2) N]).';
            elseif s(2) == 1 then
                rep = matrix(V(k),[s(1) N]);
            else
                rep = matrix(V(k), [s N]);
            end
        else
        rep = matrix(V(k), [s N]); end
    end

endfunction
