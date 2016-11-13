// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
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
//

function y = median(x,orient)
    //
    // NOTES
    //    - modified by farid.belahcene:the case when x is an hypermatrix
    //    - new syntaxes: median(x,'m') and median(x,dim)
    //    S. Gougeon 2016:
    //     - Fixes overflow issues with encoded integers when interpolating
    //       http://bugzilla.scilab.org/14640

    // CHECKING ARGUMENTS
    // ==================
    [lhs, rhs] = argn(0);

    if rhs == 0 then
        msg = _("%s: Wrong number of input argument(s): %d or %d expected.\n")
        error(msprintf(msg, "median", 1, 2))
    end

    if rhs<2 then
        orient = 0
    else
        if orient=="r" then
            orient = 1
        elseif orient=="c" then
            orient = 2
        elseif orient=="*" then
            orient = 0
        elseif orient=="m" then
            orient = find(size(x)>1,1)
            if orient==[] then
                orient = 1
            end
        else
            if type(orient)<>1|size(orient,"*")<>1|~isreal(orient)|orient<=0 then
                msg = _("%s: Wrong value for input argument #%d: ''%s'', ''%s'',''%s'' or a positive number expected.\n")
                error(msprintf(msg, "median",2,"r","c","m"))
            end
        end
    end

    // PROCESSING
    // ==========
    // median on all components
    // ------------------------
    if orient==0 then
        if x==[] then
            y = %nan
            return
        end
        n = size(x,"*");
        x = gsort(x(:),"g","i")
        if 2*int(n/2)==n then
            // avoid overflow: http://bugzilla.scilab.org/14640
            a = x(n/2)
            b = x(n/2+1)
            y = a/2 + b/2 + ((a-(a/2)*2) + (b-(b/2)*2))/2
        else
            y = x((n+1)/2);
        end

    // Projection along a given direction / dimension
    // ----------------------------------------------
    else
        if x==[] then
            y = []
            return
        end
        if orient>ndims(x) then
            y = x
            return
        end
        xsize = size(x);
        if xsize(orient)==1 then
            y = x
            return
        end
        orient_above_size = xsize(orient+1:$);
        N = prod(orient_above_size)
        orient_below_size = xsize(1:orient-1);
        M = prod(orient_below_size)
        orient_size = xsize(1:orient);
        P = prod(orient_size)
        y = [];
        n = xsize(orient)
        for k = 1:N
            for i = 1:M
                ytemp = gsort(x(i+(0:n-1)*M+(k-1)*P),"r","i")
                if 2*int(n/2)==n then
                    // avoid overflow: http://bugzilla.scilab.org/14640
                    a = ytemp(n/2)
                    b = ytemp(n/2+1)
                    y = [ y ; a/2 + b/2 + ((a-(a/2)*2) + (b-(b/2)*2))/2]
                else
                    y = [ y ; ytemp((n+1)/2)]
                end
            end
        end
        xsize(orient) = 1
        y = matrix(y, xsize)
    end
endfunction
