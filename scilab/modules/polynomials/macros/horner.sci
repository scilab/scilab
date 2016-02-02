// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [r] = horner(p,x)
    // horner(P,x) evaluates the polynomial or rational matrix P = P(s)
    // when the variable s of the polynomial is replaced by x
    // x can be a scalar or polynomial or rational matrix.
    // Example: bilinear transform; Assume P = P(s) is a rational matrix
    // then the rational matrix P((1+s)/(1-s)) is obtained by
    // horner(P,(1+s)/(1-s));
    // To evaluate a rational matrix at given frequencies use
    // preferably the freq primitive ;
    // See also: freq, repfreq.
    // Improvements:
    // Special cases aded to improve efficiency:
    // - p = row vector, x = column vector
    // - p = column vector, x = row vector
    // - x = scalar
    //!
    //
    if (argn(2) <> 2) then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"horner",2))
    end

    if (size(x, "*") == 0 | size(p, "*") == 0) then
        r = []
        return
    end

    tp = type(p)

    if (tp <= 2) then
        // tp <= 2 <=> matrix of reals, complexes or polynomials
        [m,n] = size(p)

        if (m == -1) then
            indef=%t, m=1, n=1, p=p+0
        else
            indef=%f
        end

        [mx,nx] = size(x)

        if (m*n == 1) then
            // special case: p = 1x1 polynomial, x = matrix
            cp = coeff(p)
            r = cp($) * ones(x)
            for (k = degree(p) : -1 : 1)
                r = r .* x + cp(k)
            end

        elseif (n*mx == 1)
            // p = one column, x = one row
            nd = max(degree(p));
            r = zeros(p) * x;
            for (k = nd : -1: 0)
                c = coeff(p, k);
                r = r .* (ones(p) * x) + c * ones(x);
            end

        elseif (m*nx == 1)
            // p = one row, x = one column
            nd = max(degree(p));
            r = x * zeros(p);
            for (k = nd : -1: 0)
                c = coeff(p, k);
                r = r .* (x * ones(p))+ ones(x) * c;
            end

        elseif (mx*nx == 1)
            // p = matrix, x = scalar
            nd = max(degree(p));
            r = zeros(p);
            for (k = nd : -1: 0)
                c = coeff(p, k);
                r = r * x + c;
            end

        else
            // other cases
            r = []
            for (l = 1 : m)
                rk = []
                for (k = 1 : n)
                    plk = p(l,k)
                    d = degree(plk)
                    rlk = coeff(plk,d) * ones(x); // for the case horner(1,x)
                    for (kk = 1 : d)
                        rlk = rlk .* x + coeff(plk,d-kk)
                    end
                    rk = [rk, rlk]
                end
                r = [r; rk]
            end
        end

        if (indef) then
            r = r * eye()
        end

    elseif (typeof(p) == "rational") then
        r = horner(p(2),x) ./ horner(p(3),x)

    elseif (tp == 129) then
        // implicit polynomial for indexing
        r = horner(p(:),x)
        r = r(1) : r(2) : r(3)

    else
        error(msprintf(gettext("%s: Unexpected type for input argument #%d.\n"),"horner",1))
    end

endfunction
