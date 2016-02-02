// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2013 - Samuel GOUGEON : Bug 13002 : extension to hypermatrices & integers
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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

    if isempty(m)
        i = n;
        return
    end
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
