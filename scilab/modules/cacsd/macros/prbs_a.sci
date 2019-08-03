// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function u = prbs_a(n, nc, ic)
    // n  : number of points
    // nc : number of state changes
    // ic : indices of state changes

    fname = "prbs_a"

    // Checking n
    // ----------
    if type(n)<>1 | n==[] | ~isreal(n,0) | or(n<>round(n)) then
        msg = _("%s: Argument #%d: Decimal integer expected.\n")
        error(msprintf(msg, fname, 1))
    end
    n = n(1)
    if n < 0 then
        msg = _("%s: Argument #%d: Must be >= %d.\n")
        error(msprintf(msg, fname, 1, 0))
    end
    if n==0 then
        u = []
        return
    end
    // Default ic and nc
    // -----------------
    if ~isdef("ic","l") | ic==[] then
        u = grand(1,n-1,"unf",0,1);
        [?, ic] = gsort(u);
        ic = gsort(ic(1:nc));
    else
        if type(ic)<>1 | ic==[] | ~isreal(ic,0) then
            msg = _("%s: Argument #%d: Decimal integer expected.\n")
            error(msprintf(msg, fname, 1))
        end
        if min(ic) < 1 | max(ic) >= n then
            msg = _("%s: Argument #%d: Must be in the interval %s.\n")
            error(msprintf(msg, fname, 3, msprintf("[1, %d[", n)))
        end
        nc = length(ic)
        ic = gsort(ic,"g","i");
    end
    // Checking nc
    // -----------
    if nc >= n then
        msg = _("%s: The number of switches can''t be ≥ %d = number of points.\n")
        error(msprintf(msg, fname, n))
    end
    if nc<0 then
        msg = _("%s: Argument #%d: Must be >= %d.\n")
        error(msprintf(msg, fname, 2, 0))
    end

    // PROCESSING
    // ----------
    u = zeros(1,n-1)
    u(ic(1:2:$)) = 1;
    u(ic(2:2:$)) = -1;
    u = cumsum(u);
    if min(u)==0 then
        u = [1 1-2*u];
    else
        u = [1 1+2*u];
    end
    u = u*sign(grand(1,1,"unf",-1,1));    // randomize the initial state
endfunction
