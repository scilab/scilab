// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON - Le Mans Université
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function M = polyint(M, cstes)
    // M : array (from scalar to hypermatrix) of polynomials with real or
    //     complex coefficients
    // Cstes: Integration constants (real or complex): scalar, array of size
    //     size(M), or list of mixed scalar or sized(M) arrays or
    //     undefined elements (valued as 0).

    // EXAMPLE
    // =======
    if argn(2)==0 then
        m = mode(); mode(6)
        M = [ -1+%z, 1+2*%z+%z^2 ; 2-%z^3, 6]
        mprintf("\n%s", "polyint(M)")
        I = polyint(M)
        mprintf("\n%s", "polyint(M, 1)")
        I = polyint(M, 1)
        mprintf("\n%s", "polyint(M, [1 2 ; 3 4])")
        I = polyint(M, [1 2 ; 3 4])
        mode(m)
        M = I
        return
    end

    s = size(M)
    n = length(M)

    // CHECKING ARGUMENTS
    // ==================
    rhs = argn(2)
    if type(M)<>2 then
        msg = _("%s: Argument #%d: Polynomial expected.\n")
        error(msprintf(msg, "polyint",1))
    end
    if ~isdef("cstes","l")
        cstes = 0
    end
    if and(type(cstes)<>[1 15])
        msg = _("%s: Argument #%d: numbers or list of numbers expected.\n")
        error(msprintf(msg, "polyint",2))
    end
    if type(cstes) <> 15 then
        cstes = list(cstes)
    end
    for i = 1:length(cstes)
        k = cstes(i)
        if isdef("k","l")
            if type(k) <> 1
                msg = _("%s: Argument #%d(%d): Decimal or complex number expected.\n")
                error(msprintf(msg, "polyint",2,i))
            end
            if length(k)<>1 & or(size(k)<>s)
                msg = _("%s: Argument #%d(%d): Array of size [%s] expected.\n")
                ts = strcat(msprintf("%d\n",s(:))," ")
                error(msprintf(msg, "polyint", 2, i, ts))
            end
        end
    end

    // PROCESSING
    // ==========
    vn = poly(0, varn(M))
    M = matrix(M, -1, 1)
    for i = cstes
        if ~isdef("i","l")
            i = zeros(n,1)
        elseif length(i)==1
            i = i * ones(n,1)
        else
            i = matrix(i,-1,1)
        end
        M = M * vn
        d = max(max(degree(M),0))
        p = (0:d) .*. ones(M)
        c = coeff(M)
        k = find(p > 0)
        c(k) = c(k) ./ p(k)
        c(:,1) = c(:,1) + i
        M = inv_coeff(c, d, varn(M))
    end

    M = matrix(M, s)
endfunction
