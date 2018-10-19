// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) CNES - 2011 - Guillaume AZEMA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2016, 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = %i_linspace(d1, d2, n)
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
    if type(d1)==8 & type(d2)==8 & inttype(d1)~=inttype(d2) then
        msg = gettext("%s: Arguments #%d and #%d: Same integer types expected.\n")
        error(msprintf(msg, "linspace", 2));
    end
    msg = gettext("%s: Argument #%d: %%nan and %%inf values are forbidden.\n")
    if type(d1)~=8 & (or(isinf(d1)) | or(isnan(d1))) then
        error(msprintf(msg, "linspace", 1));
    end
    if type(d2)~=8 & (or(isinf(d2)) | or(isnan(d2))) then
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
    if n==1
        y = d2
    elseif n==2
        y = [d1 d2]
    elseif n>2
        if or(inttype(d1)==[8 18])
            y = linspace_integers_64(d1,d2,n)
        else
            if type(d1)==8
                span = double(d2) - double(d1)
            else
                span = d2 - d1
            end
            y = (span * (0:n-1)) / (n-1) + d1 * ones(1,n);
        end
        // Forces the last value to be exactly the given d2:
        // http://bugzilla.scilab.org/10966
        y(:,$) = d2;
    else
        y = []
    end
endfunction
// -----------------
function y = linspace_integers_64(d1,d2,n)
    s = d2>=d1
    span = zeros(d1)
    if typeof(d1)=="int64"  then
        span = uint64(span)
    end
    span(s) = d2(s) - d1(s)
    span(~s) = d1(~s) - d2(~s)
    step = span/(n-1)
    y = iconvert(zeros(size(d1,1),n), inttype(d1))
    if or(s)  // d2 > d1
        y(s,:) = d1(s)*ones(1,n) + step(s)*(0:n-1)
    end
    if or(~s)
        y(~s,:) = d1(~s)*ones(1,n) - step(~s)*(0:n-1)
    end
    y(:,$) = d2
    // Computing the balancing corrections
    // We computes actual intervals
    if typeof(d1)=="uint64" then
        i = int64(zeros(size(d1,1),n-1))
        i(s, :) = y(s, 2:$)    -  y(s, 1:$-1)
        i(~s,:) = y(~s, 1:$-1) -  y(~s, 2:$)
    else // int64
        i = diff(y,1,2);
    end
    // diff wrt the 1st interval's width
    e = i - i(:,1) * ones(1,n-1)

    // Computing corrections
    S = mean(double(e), "c") * ones(1,n-1)
    cumE = cumsum(e, "c")
    cumS = cumsum(S, "c")
    delta = cumS - cumE
    // Applying the corrections
    y(:,2:$) = y(:,2:$) + delta
endfunction
