// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2013, 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function i = pmodulo(n, m)

    [lhs, rhs] = argn(0);
    if rhs <> 2 then
        msg = _("%s: Wrong number of input argument(s): %d expected.\n")
        error(msprintf(msg, "pmodulo", 2))
    end

    mt = type(m)
    nt = type(n)

    // -----------------------  Checking arguments --------------------------

    if and(nt <> [1 2 8]) | (nt==1 & ~isreal(n)) then
        msg = _("%s: Wrong type for input argument #%d: Real, integer or polynomial matrix expected.\n")
        error(msprintf(msg, "pmodulo", 1))
    end

    if and(mt <> [1 2 8]) | (mt==1 & ~isreal(m)) then
        msg = _("%s: Wrong type for input argument #%d: Real, integer or polynomial matrix expected.\n")
        error(msprintf(msg, "pmodulo", 2))
    end

    if (nt==8 | mt==8)  & nt~=mt
        msg = _("%s: Incompatible input arguments: Same types expected.\n")
        error(msprintf(msg, "pmodulo"))
    end

    // --------------------------  Processing ------------------------

    if m==[]
        i = n;
        return;
    end
    if  nt==2 then
        [i,?] = pdiv(n, m)
    else
        m = abs(m)  // else returns i<0 for m<0 : http://bugzilla.scilab.org/12373
        if length(n)>1 & length(m)>1 & or(size(n)<>size(m)) then
            msg = _("%s: Wrong size for input arguments: Same size expected.\n")
            error(msprintf(msg, "pmodulo"))
        end
        i = n - floor(n ./ m) .* m
        k = find(i<0)           // this may occur for encoded integers
        if k~=[]
            if length(m)>1 then
                i(k) = i(k) + m(k)
            else
                i(k) = i(k) + m
            end
        end
        i = iconvert(i, inttype(n))
    end

endfunction
