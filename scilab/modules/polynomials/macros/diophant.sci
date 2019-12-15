// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x, err] = diophant(p1p2, b)
    //solves diophantine equation p1*x1+p2*x2 = b
    //with  p1p2 a polynomial vector [p1 p2]
    //b polynomial
    //x polynomial vector [x1;x2]
    //!

    err = 0
    x = []

    if or(type(p1p2)==[1 2 5]) & (or(isinf(p1p2)) | or(isnan(p1p2))) then
        msg = _("%s: Argument #%d: Inf or NaN coefficients not supported.\n")
        error(msprintf(msg, "diophant", 1))
    end
    [p1, p2] = (p1p2(1), p1p2(2))

    if or(type(b)==[1 2 5]) & (isinf(b) | isnan(b)) then
        msg = _("%s: Argument #%d: Inf or NaN coefficients not supported.\n")
        error(msprintf(msg, "diophant", 2))
    end

    // PROCESSING
    // ----------
    if p1==0 & p2==0
        if b <> 0
            err = %nan     // No solution
        else
            x = [1 1] + p1 // + p1 => forces the result to the p1 type
            err = -%inf     // Infinite set of solutions
        end
    else
        if p1==0 | p2==0 then
            err = -%inf          // Infinite set of solutions
        end

        [Gcd, u] = bezout(p1, p2)
        if type(p1)==1          // http://bugzilla.scilab.org/16282
            Gcd = coeff(Gcd)
            u = coeff(u)
        end
        if type(p1)==1 | (type(p1)==2 & and(degree([p1 p2 b])<=0))
            err = -%inf
        end
        if b == 0 then
            x = u(:,2)
        else
            x = u(:,1) * b
            if type(x)<>2 & type(Gcd)<>2
                x = x / Gcd
            else
                x = clean(x, %eps)
                Gcd = clean(Gcd, %eps)
                [r, x] = pdiv(x, Gcd)
                if degree(Gcd)>0 & or(clean(r,%eps)<>zeros(r))
                    x = []
                    [r, q] = pdiv(b, Gcd)
                    if err <> -%inf
                        err = norm(coeff(b - Gcd*q),2) / norm(coeff(b), 2)
                    end
                end
            end
        end
    end

    // FINAL FORMATING AND CASTING
    // ---------------------------
    if x <> [] then
        x = matrix(x, size(p1p2))
        if type(p1p2) == 8
            if sum(p1p2.*x) <> b    // check against inttype rounding
                x = []
                err = abs((double(b) - double(sum(p1p2.*x)))/double(b))
            else
                x = iconvert(x, inttype(p1p2))
            end
        elseif type(p1p2) <> 2
            x = iconvert(x, inttype(p1p2))
        else
            if type(x) <> 2
                x = double(x) + 0*p1
            end
        end
    end
endfunction
