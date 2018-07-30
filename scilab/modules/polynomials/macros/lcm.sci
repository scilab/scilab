// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) INRIA - Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2017 - 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [q, fact] = lcm(p)
    //q = lcm(p) computes the Least Common Multiple of vector p of polynomials
    // or integers
    //[q, fact] = lcm(p) computes the vector fact of factors such that
    // p .* fact = q*ones(p)

    if ~or(type(p)==[1 2 8]) then
        msg = _("%s: Wrong type for argument #%d: Array of integers or polynomials expected.\n")
        error(msprintf(msg, "lcm", 1))
    end

    if  type(p)~=2
        intype = inttype(p)
        if type(p)==1 then
            if floor(p)<>p
                msg = _("%s: Wrong values for argument #%d: Integer values expected.\n")
                error(msprintf(msg, "lcm", 1));
            end
        else
            p = iconvert(p,0);
        end
    end

    // Integers
    // --------
    if type(p)==1 then
        k = find(p==0)
        if k<>[] then
            q = 0
            fact = zeros(p)
            fact(k) = 1
        else
            q = p(1)
            for k = 2:size(p,"*")
                q = round(q / gcd([q,p(k)]))*p(k)
            end
            fact = round(q ./ p)
        end
        if q~=[] & q<0 then
            q = -q
            fact = -fact;
        end
        return
    end

    // Polynomials
    // -----------
    [m, n] = size(p),
    p = matrix(p, m*n, 1),
    p0 = p(1);
    fact = 1;
    for l = 2:m*n,
        [u, v] = simp(p0, p(l))
        p0 = p0*v,
        fact = [v*fact, u],
    end,
    fact = matrix(fact, m, n)
    q = p0;
endfunction
