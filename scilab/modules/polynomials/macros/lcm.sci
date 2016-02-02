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


function [p, fact] = lcm(p)
    //p=lcm(p) computes the lcm of polynomial vector p
    //[pp,fact]=lcm(p) computes besides the vector fact of factors
    //such that  p.*fact=pp*ones(p)
    //!

    if type(p)<>1 & type(p)<>2 & type(p)<>8 then
        error(msprintf(_("%s: Wrong type for argument #%d: Integer array or Polynomial expected.\n"), "lcm", 1));
    end

    if type(p)==1 then
        if floor(p)<>p then
            error(msprintf(_("%s: Wrong type for argument #%d: Integer array or Polynomial expected.\n"), "lcm", 1));
        else
            p = iconvert(p,4);
        end
    end

    if type(p)==8 then
        if argn(1)==2 then [p, fact] = %i_lcm(p), else p = %i_lcm(p), end
        return
    end

    [m, n] = size(p),
    p = matrix(p, m*n, 1),
    p0 = p(1); fact = 1;
    for l = 2:m*n,
        [u, v] = simp(p0, p(l)),
        p0 = p0*v,
        fact = [v*fact, u],
    end,
    fact = matrix(fact, m, n),
    p = p0;

endfunction
