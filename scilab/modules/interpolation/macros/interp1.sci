// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2021 - Samuel GOUGEON - Le Mans Université
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function yi = interp1(varargin)
    // yi = interp1(x, y, xi)
    // yi = interp1(x, y, xi, method)
    // yi = interp1(x, y, xi, method, extrapolation)
    // yi = interp1(y, xi)
    // yi = interp1(y, xi, method)
    // yi = interp1(y, xi, method, extrapolation)
    //
    // x: real vector
    // y: real or complex vector of length(x) or array of size(y,1)==length(x)
    // xi: real scalar, vector, matrix or hypermatrix of any size.
    // method = "linear" (default) | "nearest" | "spline"
    // extrapolation = "extrap" (= method) | real or complex padding scalar |
    //  * if method = "linear": "linear" | "periodic" | "edgevalue"
    //  * if method = "spline": "linear" | "periodic" | "edgevalue" | "spline"
    //  * if method = "nearest": "nearest" = "edgevalue"

    // ========================
    // CHECKING INPUT ARGUMENTS
    // ========================
    rhs = size(varargin)
    // 2 <= Number of input arguments <= 5
    if rhs<2 | rhs>5 then
        msg = _("%s: Wrong number of input arguments: Must be between %d and %d.\n")
        error(msprintf(msg, "interp1", 2, 5));
    end

    // Checking x
    // ----------
    iy = 2  // index of y argin, for error messages
    if rhs >= 3 & type(varargin(1))<>0 & type(varargin(3))<>10 then
        // x is explicit: interp1(x, y, xi,..)
        x = varargin(1)
        if type(x)<>1 | ~isreal(x) then
            msg = _("%s: Argument #%d: Real numbers expected.\n")
            error(msprintf(msg,"interp1",1))
        elseif ~isvector(x)
            msg = _("%s: Argument #%d: Vector expected.\n")
            error(msprintf(msg, "interp1", 1))
        elseif or(isnan(x)) then
            msg = _("%s: Argument #%d: Nan value forbidden.\n")
            error(msprintf(msg, "interp1", 1))
        end
        x = matrix(x,1,-1)
        y = varargin(2)
    else // Default x: 
        if type(varargin(1))<>0 // interp1(y, xi,..)
            iy = 1
            y = varargin(1)
        else                    // interp1(, y, xi,..)
            y = varargin(2)
        end
        if isvector(y) then
            x = 1:size(y,"*")
            y = y(:)
        else
            x = 1:size(y,1)
        end
    end
    if length(x) <= 1
        if iy==2
            msg = _("%s: Argument #%d: At least 2 elements expected.\n")
            error(msprintf(msg, "interp1", 1))
        else
            msg = _("%s: Argument #%d: vector or at least 2 rows expected.\n")
            error(msprintf(msg, "interp1", 2))
        end
    end
    // Sorting x in increasing order:
    [?, k] = gsort(x,"g","i")
    x = x(k)    // should anyway be a row

    // Checking y
    // ----------
    if type(y) <> 1 then
        msg = _("%s: Argument #%d: Decimal or complex numbers expected.\n")
        error(msprintf(msg, "interp1", iy));
    end
    if isvector(y) then
        y = y(:)
        if length(y) <> length(x)
            msg = _("%s: Arguments #%d and #%d: Same numbers of elements expected.\n")
            error(msprintf(msg, "interp1", 1, 2))
        end
    else
        if size(y,1) <> length(x) then
            msg = _("%s: Arguments #%d and #%d: Same numbers of rows expected.\n")
            error(msprintf(msg, "interp1", 1, 2))
        end
    end
    // Sorting y according to x:
    cmd = "y = y(k," + strcat(repmat(":",[1 ndims(y)-1]),",")+")"
    execstr(cmd)

    // Checking xi
    // -----------
    if type(varargin(iy+1))<>1 | ~isreal(varargin(iy+1)) then
        msg = _("%s: Argument #%d: Decimal numbers expected.\n")
        error(msprintf(msg, "interp1", iy+1))
    end
    xi = varargin(iy+1)
    if xi == [] then
        yi = []
        return
    end
    xisize = size(xi)
    if length(size(y))==2 & or(size(y)==1) then  // y = 2D vector
        if iscolumn(xisize) then
            xisize = length(xi)
        end
    else
        if length(size(xi))==2 & or(size(xi)==1) then // xi = scalar or 2D vector
            xisize = length(xi)
        end
    end

    // Checking method
    // ---------------
    if rhs > iy+1 & type(varargin(iy+2))<>0 then
        method = varargin(iy+2)
        if size(method,"*") <> 1
            msg = _("%s: Argument #%d: Scalar (1 element) expected.\n")
            error(msprintf(msg, "interp1", iy+2))
        end
        if type(method) <> 10 | ~or(convstr(method)==["linear","nearest","spline"])
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, "interp1", iy+2, """linear"",""spline"",""nearest"""))
        end
        method = convstr(method)
    else
        method = "linear"
    end

    // Checking extrapolation method
    // -----------------------------
    extraVal = []
    if rhs > iy+2 then
        extra = varargin(iy+3)
        err = %f
        if type(extra)==10 & size(extra,"*")==1
            extra = convstr(extra)
            if extra=="extrap"
                extra = "natural"
            elseif or(method==["linear" "spline"])
                if extra=="linear" then
                    if method=="linear"
                        extra = "natural"
                    end
                elseif extra=="edgevalue"
                    extra = "C0"
                elseif extra=="periodic"
                    msg = []
                    if isvector(y) & y($)<>y(1)
                        msg = _("%s: Argument #%d: periodicity y($)==y(1) expected.\n")
                    elseif ~isvector(y) & or(y($,:)<>y(1,:))
                        msg = _("%s: Argument #%d: periodicity y($,:)==y(1,:) expected.\n")
                    end
                    if msg <> []
                        error(msprintf(msg, "interp1",iy))
                    end
                elseif extra<>"natural"
                    err = %t
                end
            else // nearest
                if or(extra==["edgevalue" "nearest" "natural"])
                    extra = "natural"
                else
                    err = %t
                end
            end
        else
            if type(extra)<>1 | length(extra)<>1
                err = %t
            else
                extraVal = extra
                extra = "natural"
            end
        end
        if err
            if method=="nearest"
                tmp = """extrap"",""edgevalue"",""nearest"""
            else
                tmp = """extrap"",""linear"",""periodic"",""edgevalue"""
            end
            msg = _("%s: Argument #%d: scalar number or one of {%s} expected.\n")
            error(msprintf(msg, "interp1", iy+3, tmp))
        end
    else
        extra = "natural"
        if method <> "spline"
            extraVal = %nan
        end
    end

    // ==========
    // PROCESSING
    // ==========
    if ~isreal(y) then
        if extraVal==[]
            [extraR, extraI] = (extra, extra);
        else
            [extraR, extraI] = (real(extraVal), imag(extraVal));
        end
        yir = interp1(x, real(y), xi, method, extraR)
        yii = interp1(x, imag(y), xi, method, extraI)
        yi = complex(yir, yii)
        return
    end
    // REAL CASE
    // =========
    select part(method, 1)
    case "l"
        // LINEAR
        // ------
        xitemp = matrix(xi,-1,1)
        // y is a vector
        if isvector(y) then
            yi = resize_matrix(0, size(xitemp))
            if extraVal <> [] then
                yi = linear_interpn(xitemp, x, y, "by_nan");
                if ~isnan(extraVal) then
                    k = find(xitemp<min(x) | xitemp>max(x))
                    yi(k) = extraVal
                end
            else
                yi = linear_interpn(xitemp, x, y, extra);
            end
            yi = matrix(yi, xisize)

        else    // y is matrix or hypermatrix
            ky = size(y)(2:$)
            ncolumns = prod(ky)
            yi = resize_matrix(0, [size(xitemp) ky])
            if extraVal <> [] then
                for ic = 1:ncolumns
                    tmp = linear_interpn(xitemp, x, y(:,ic), "by_nan")
                    yi(:,ic) = matrix(tmp, size(xitemp))
                end
                if ~isnan(extraVal) then
                    k = find(xitemp < min(x) | xitemp > max(x))
                    yi(k,:) = extraVal
                end
            else // extra=="linear"=="natural" | "periodic" | "edgevalue"=="C0"
                for ic = 1:ncolumns
                    tmp = linear_interpn(xitemp, x, y(:,ic), extra)
                    yi(:,ic) = matrix(tmp, size(xitemp))
                end
            end
            yi = matrix(yi, [xisize, ky])
        end

    case "s"
        // SPLINE
        // ------
        xitemp = matrix(xi,-1,1)
        // y is a vector
        if isvector(y) then
            yi = resize_matrix(0, size(xitemp))
            yrow = matrix(y,1,-1)
            if extra=="linear"
                yi = interp(xitemp, x, yrow, splin(x, yrow, "natural"), extra)
            elseif extra=="periodic"
                yi = interp(xitemp, x, yrow, splin(x, yrow, extra), extra)
            else    // default not_a_knot
                yi = interp(xitemp, x, yrow, splin(x, yrow), extra)
            end
            if extraVal <> []  then
                k = find(xitemp<min(x) | xitemp>max(x))
                yi(k) = extraVal
            end
            yi = matrix(yi, xisize)

        else    // y is a matrix or a hypermatrix
            ky = size(y)(2:$)
            ncolumns = prod(ky)
            yi = resize_matrix(0, [size(xitemp) ky])
            x  = matrix(x,-1,1)
            xi = matrix(xi,-1,1)
            for ic = 1:ncolumns
                tmp = y(:,ic)
                tmp = interp(xi, x, tmp, splin(x, tmp), extra)
                yi(:,ic) = matrix(tmp, size(xitemp))
            end
            // extrapolation
            if extraVal <> []  then
                k = find(xitemp<min(x) | xitemp>max(x))
                yi(k,:) = extraVal
            end
            yi = matrix(yi,[xisize,ky])
        end

    case "n"
        // NEAREST
        // -------
        // if all xi values are nan, retuns nan values for yi
        if and(isnan(xi)) then
            yi = xi
            return
        end

        xitemp = matrix(xi,1,-1)
        knan = isnan(xitemp)
        [xitemp,p] = gsort(xitemp(~knan),"g","i")
        // For each xi, we search for the k index of the x nearest to xi:
        k = zeros(xitemp);
        nx = length(x);
        j = length(xitemp);
        i = nx;
        while j>=1 & i>=1
            if xitemp(j) >= x(i)  then
                if i <> nx then
                    k(j) = i;
                end
                j = j - 1;
            else
                i = i - 1;
            end
        end
        k(xitemp < x(1)) = 1;
        k(xitemp >= x(nx)) = nx - 1; 
        i = find(xitemp >= matrix((x(k)+x(k+1))/2,size(k)));
        if i~=[]
            k(i) = k(i)+1;
        end

        // y is vector
        if isvector(y) then
            yi = y(k)
            yi = matrix(yi,1,-1)
            // extrapolation
            if extraVal <> [] then
                n = find(xitemp<min(x) | xitemp>max(x) | isnan(xitemp))
                yi(n) = extraVal
            end
            yitemp = yi
            yi(p)  = yitemp
            ytemp  = yi
            yi = matrix(xi,1,-1)
            yi(knan) = %nan
            yi(~knan) = ytemp
            yi = matrix(yi, xisize)

        else  // y is a matrix or a hypermatrix
            ky = size(y)(2:$)
            yi = resize_matrix(0, [size(xitemp,"*") ky])
            ncolumns = prod(ky)
            for ic = 1:ncolumns
                yi(:,ic) = y(k,ic)
            end
            // extrapolation
            if extraVal <> [] then
                n = find(xitemp < min(x) | xitemp > max(x))
                for ic = 1:ncolumns
                    yi(n,ic) = extraVal
                end
            end
            yitemp = yi
            for ic = 1:ncolumns
                yi(p,ic) = yitemp(:,ic)
            end
            yitemp = yi
            yi = resize_matrix(0, [size(xi,"*") ky])
            for ic = 1:ncolumns
                yi(knan,ic) = %nan
                yi(~knan,ic) = yitemp(:,ic)
            end
            yi = matrix(yi,[xisize,ky])
        end
    end
endfunction
