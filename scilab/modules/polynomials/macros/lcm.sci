// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) INRIA - Serge Steer
// Copyright (C) 2017 - Samuel GOUGEON : http://bugzilla.scilab.org/15058
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [p, fact] = lcm(p)
    //p=lcm(p) computes the lcm of polynomial vector p
    //[pp,fact]=lcm(p) computes besides the vector fact of factors
    //such that  p.*fact=pp*ones(p)
    //!

    if type(p)<>1 & type(p)<>2 & type(p)<>8 then
        msg = _("%s: Wrong type for argument #%d: Integer array or Polynomial expected.\n");
        error(msprintf(msg, "lcm", 1));
    end

    if type(p)==1 then
        if floor(p) <> p then
            msg = _("%s: Wrong type for argument #%d: Integer array or Polynomial expected.\n");
            error(msprintf(msg, "lcm", 1));
        else
            p = iconvert(p, 4);
        end
    end

    fact = 1;

    // Integers:
    if type(p)==8 then
        if argn(1)==2 then
            [p, fact] = %i_lcm(p);
        else
            p = %i_lcm(p);
        end
        if p~=[] & p<0 then
            p = -p
            fact = -fact;
        end
        return
    end

    // Polynomials:
    [m, n] = size(p),
    p = matrix(p, m*n, 1),
    p0 = p(1);
    for l = 2:m*n,
        [u, v] = simp(p0, p(l)),
        p0 = p0*v,
        fact = [v*fact, u],
    end,
    fact = matrix(fact, m, n),
    p = p0;
endfunction

// ----------------------------------------------------------------------------

function [q, fact] = %i_lcm(p)
    // p = lcm(p) computes the lcm of polynomial vector p
    // [pp,fact]=lcm(p) computes besides the vector fact of factors
    // such that  p.*fact=pp*ones(p)

    k = find(p==0);
    if k <> [] then
        q = p(k(1));
        fact = 0*ones(p);
        fact(k) = 1;
        return
    end

    q = p(1);
    for k = 2:size(p,"*")
        q = q / gcd([q,p(k)]) * p(k);
    end
    fact = q ./ p;
endfunction
