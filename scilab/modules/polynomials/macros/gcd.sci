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


function [x, uu] = gcd(p)
    //Given a polynomial vector p, [pgcd,u]=gcd(p) computes the gcd
    //of components and a unimodular matrix (with polynomial inverse) u,
    //with minimal degree such that [p1 p2]*u=[0 ... 0 pgcd]
    //!

    if type(p)<>1 & type(p)<>2 & type(p)<>8 then
        error(msprintf(_("%s: Wrong type for argument #%d: Integer array or Polynomial expected.\n"), "gcd", 1));
    end

    if type(p)==1 then
        if floor(p)<>p then
            error(msprintf(_("%s: Wrong type for argument #%d: Integer array or Polynomial expected.\n"), "gcd", 1));
        else
            p = iconvert(p,4);
        end
    end

    [lhs,rhs]=argn(0)
    if type(p)==8 then
        if lhs==2 then [x,uu]=%i_gcd(p), else x=%i_gcd(p), end
        return
    end

    [m, n] = size(p)
    mn = m*n
    p = matrix(p, 1, mn)
    x = p(1);
    uu = 1
    for l = 2:mn,
        [x, u] = bezout(x, p(l)),
        if lhs==2 then
            uu = [uu(:, 1:l-2) uu(:, l-1)*u(1, [2 1])]; uu(l, l-1:l) = u(2, [2 1]);
        end
    end,
    if lhs==1 then return end
    for l = mn:-1:2
        pivot = uu(l, l-1);
        for k = l:mn
            [r, q] = pdiv(uu(l, k), pivot)
            if coeff(q)<>0 then
                uu(1:l-1, k) = uu(1:l-1, k)-q*uu(1:l-1, l-1)
                uu(l, k) = r;
            end
        end
    end

endfunction
