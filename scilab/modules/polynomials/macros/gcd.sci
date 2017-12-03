// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2017 - Samuel GOUGEON : http://bugzilla.scilab.org/15017
//                                       http://bugzilla.scilab.org/15058
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

    [lhs,rhs] = argn(0)
    if rhs~=1
        msg = _("%s: Wrong number of input arguments: %d expected.\n")
        error(msprintf(msg, "gcd", 1));
    end
    if ~or(type(p)==[1 2 8])
        msg = _("%s: Wrong type for argument #%d: Decimal or encoded integers or Polynomial expected.\n")
        error(msprintf(msg, "gcd", 1));
    end

    if  type(p)~=2
        intype = inttype(p)
        if type(p)==1
            if floor(p)<>p
                msg = _("%s: Wrong values for argument #%d: Integer values expected.\n")
                error(msprintf(msg, "gcd", 1));
            else
                p = iconvert(p,8);  // calculations may need/lead to negative terms (argout#2)
            end
        end
    end


    mn = size(p,"*")
    p = matrix(p, 1, -1)
    x = p(1);
    uu = 1
    for l = 2:mn,
        [x, u] = bezout(x, p(l)),
        if lhs==2 then
            uu = [uu(:, 1:l-2) uu(:, l-1)*u(1, [2 1])];
            uu(l, l-1:l) = u(2, [2 1]);
        end
    end

    if lhs>1
        if type(p)==2
            // Polynomials case
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
        else
            // Integers
            iz = iconvert(0,intype)
            for l = mn:-1:2
                pivot = uu(l, l-1);
                for k=l:mn
                    q = uu(l,k)/pivot
                    r = uu(l,k)-q*pivot
                    if q <> iz then
                        uu(1:l-1,k)=uu(1:l-1,k)-q*uu(1:l-1,l-1)
                        uu(l,k)=r;
                    end
                end
            end
        end
    end
    if type(p)~=2 & x~=[] & x<0 then
        x = -x;
        uu(:,$) = -uu(:,$);
    end
    if type(p)==8
        x  = iconvert(x, intype)
        uu = iconvert(uu, modulo(intype,10))    // may have negative terms
    end

endfunction
