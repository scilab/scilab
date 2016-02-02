// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Bruno Pincon
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
function [y] = detrend(x, flag, bp)
    //
    // this function removes the constant or linear or
    // piecewise linear trend from a vector x. If x is
    // matrix this function removes the trend of each
    // column of x.
    //
    // flag = "constant" or "c" to removes the constant trend
    //        (simply the mean of the signal)
    // flag = "linear" or "l" to remove the linear or piecewise
    //        linear trend.
    //
    // To define the piecewise linear trend the function needs the
    // breakpoints and these must be given as the third argument bp.
    //
    // The "instants" of the signal x are in fact from 0 to m-1
    // (m = length(x) if x is a vector and m = size(x,1) in case
    // x is a matrix). So bp must be reals in [0 m-1].
    //

    rhs = argn(2)
    if rhs < 1 | rhs > 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"detrend",1,3));
    elseif rhs == 1
        flag = "linear"; bp = []
    elseif rhs == 2
        bp = []
    end

    if type(x)~=1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"detrend",1));
    end
    if type(flag)~=10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"detrend",2));
    end
    if ~(type(bp)==1 & isreal(bp)) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"detrend",3));
    end

    [mx,nx] = size(x)
    if mx==1 | nx==1 then
        x_is_vector = %t; x = x(:); m = mx*nx; n = 1
    elseif mx*nx == 0 then
        y = []; return
    else
        x_is_vector = %f; m = mx; n = nx
    end


    if flag == "constant" | flag == "c" then
        y = x - ones(m,1)*mean(x,"r")
    elseif flag == "linear" | flag == "l"
        bp = unique([0 ; bp(:) ; m-1])
        // delete all the breakpoints outside [0,m-1]
        while bp(1) < 0, bp(1)=[], end
        while bp($) > m-1, bp($)=[], end
        // breakpoints are 0-based so add one to
        // compare them with signal vector indices (1-based)
        bp = bp + 1;
        nbp = length(bp);
        // build the least square matrix with hat functions
        // (as a basis for continuous piecewise linear functions)
        A = zeros(m, nbp)
        k1 = 1
        delta_bp = diff(bp)
        for j = 2:nbp-1
            k2 = ceil(bp(j))-1
            ind = (k1:k2)'
            A(ind,j-1) = (bp(j) - ind)/delta_bp(j-1)
            A(ind,j) = (ind - bp(j-1))/delta_bp(j-1)
            k1 = k2+1
        end
        ind = (k1:m)'
        A(ind,nbp-1) = (m - ind)/delta_bp(nbp-1)
        A(ind,nbp) = (ind - bp(nbp-1))/delta_bp(nbp-1)
        // solve the least square pb and retrieve the fitted
        // piecewise linear func off the signal
        y = x - A*(A\x)
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n") ,..
        "detrend",2,"''constant'',''c'',''linear'',''l''"));
    end

    if x_is_vector then
        y = matrix(y,mx,nx)
    end

endfunction
