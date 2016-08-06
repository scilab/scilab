// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) CNES - 2011 - Guillaume AZEMA
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = linspace(d1, d2, n)
    // Linearly spaced vector.
    // linspace(x1, x2) generates a row vector of 100 linearly
    // equally spaced points between x1 and x2.
    // linspace(x1, x2, n) generates n points between x1 and x2.

    // CHECKING ARGUMENTS
    // ------------------
    rhs = argn(2);
    if rhs < 2 then
        msg = gettext("%s: Wrong number of input argument(s): %d to %d expected.\n")
        error(msprintf(msg, "linspace", 2, 3));
    end
    if size(d1,2)<>1 then
        msg = gettext("%s: Argument #%d: Column expected.\n")
        error(msprintf(msg, "linspace", 1));
    end
    if ~and(size(d1) == size(d2)) then
        msg = gettext("%s: Arguments #%d and #%d: Same sizes expected.\n")
        error(msprintf(msg, "linspace", 1, 2));
    end
    msg = gettext("%s: Argument #%d: Number(s) expected.\n")
    if ~or(type(d1)==[1 5 8]) then
        error(msprintf(msg, "linspace", 1));
    end
    if ~or(type(d2)==[1 5 8]) then
        error(msprintf(msg, "linspace", 2));
    end
    msg = gettext("%s: Argument #%d: %%nan and %%inf values are forbidden.\n")
    if or(isinf(d1)) | or(isnan(d1)) then
        error(msprintf(msg, "linspace", 1));
    end
    if or(isinf(d2)) | or(isnan(d2)) then
        error(msprintf(msg, "linspace", 2));
    end

    if rhs == 2 then
        n = 100;
    else
        if and(type(n)<>[1 8]) | size(n,"*")<>1 | int(n)<>n then
            msg = gettext("%s: Argument #%d: An integer value expected.\n")
            error(msprintf(msg, "linspace",3));
        end
        n = double(n); // Convert for the operations to come
    end

    // PROCESSING
    // ----------
    if n>1
        y = ((d2-d1) * (0:n-1)) / (n-1) + d1 * ones(1,n);
        // Forces the last value to be exactly the given d2:
        // http://bugzilla.scilab.org/10966
        y(:,$) = d2;
    elseif n==1
        y = d2
    else
        y = []
    end
endfunction
