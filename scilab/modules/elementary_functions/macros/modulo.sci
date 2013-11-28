// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2013 - Samuel GOUGEON : Bug 13002 : extension to hypermatrices & integers
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function i = modulo(n, m)

    [lhs, rhs] = argn(0);
    if rhs <> 2 then
        msg = _("%s: Wrong number of input argument(s): %d expected.\n")
        error(msprintf(msg, "modulo", 2))
    end

    mt = type(m($))
    nt = type(n($))
    // -----------------------  Checking arguments --------------------------

    if or(type(n)==[15 16]) | and(nt <> [1 2 8]) | (nt==1 & ~isreal(n))
        msg = _("%s: Wrong type for input argument #%d: Real, integer or polynomial matrix expected.\n")
        error(msprintf(msg, "modulo", 1))
    end

    if or(type(m)==[15 16]) |  and(mt <> [1 2 8]) | (mt==1 & ~isreal(m))
        msg = _("%s: Wrong type for input argument #%d: Real, integer or polynomial matrix expected.\n")
        error(msprintf(msg, "modulo", 2))
    end

    if (nt==8 | mt==8)  & nt~=mt
        msg = _("%s: Incompatible input arguments #%d and #%d: Same types expected.\n")
        error(msprintf(msg, "modulo", 1, 2))
    end

    // --------------------------  Processing ----------------------------

    if or(mt==[1 8]) & mt==nt then
        ms = size(m)
        ns = size(n)
        m = m(:)
        n = n(:)
        if length(n)>1 & length(m)>1 & or(ns <> ms) then
            msg = _("%s: Wrong size for input arguments: Same size expected.\n")
            error(msprintf(msg, "modulo"))
        end
        i = n - int(n ./ m) .* m
        i = iconvert(i, inttype(n))
        if length(m)>1 then
            s = ms
        else
            s = ns
        end
        i = matrix(i, s)
    else
        [i,q] = pdiv(n, m)
    end

endfunction
