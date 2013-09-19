// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function [p, fact] = lcm(p)
    //p=lcm(p) computes the lcm of polynomial vector p
    //[pp,fact]=lcm(p) computes besides the vector fact of factors
    //such that  p.*fact=pp*ones(p)
    //!

    if (type(p)<>2 & type(p)<>8) then
        error(msprintf(_("%s: Wrong type for argument #%d: Integer array or Polynomial expected.\n"), "lcm", 1));
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
