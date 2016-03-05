// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y=logspace(d1, d2, n)
    // Logarithmically spaced vector.
    // logspace(d1, d2) generates a row vector of n logarithmically
    // equally spaced points between 10^d1 and 10^d2.  If d2=%pi
    // then the points are between 10^d1 and pi.
    // logspace(d1, d2, n) generates n points.

    rhs = argn(2);
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"logspace", 2));
    end
    if size(d1,2)<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A column vector expected.\n"),"logspace",1));
    end

    if ~and(size(d1) == size(d2)) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"logspace",1,2));
    end

    if rhs == 2 then
        n = 50;
    else
        if type(n)<>1|size(n,"*")<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: An integer value expected.\n"),"logspace",3));
        end
        if int(n) <> n then
            warning(msprintf("%s: Using a non integer value for input argument #%d is deprecated and will produce an error in version 5.4.1.\n","logspace",3));
        end
    end
    if d2==%pi then
        d2 = log10(%pi);
    end
    y=10 .^( d1*ones(1,n) + [(d2-d1)*(0:n-2)/(floor(n)-1),d2-d1]);
endfunction
