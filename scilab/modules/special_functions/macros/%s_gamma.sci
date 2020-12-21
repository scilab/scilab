// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = %s_gamma(varargin)
    // call for complex numbers or/and for hypermatrix of reals
    //      or/and for incomplete integral
    // gamma(a)          // with a complex or real hypermat
    // gamma(x, a)       // with real x >= 0
    // gamma(x, a, b)
    // gamma(x,.., option)  // "upper": complementary integral

    msg = _("%s: Function not defined for the given argument type.\n  Check arguments or define function %s for overloading.\n")
    rhs = argn(2)
    a = varargin(min(2,rhs))
    sa = size(a)
    y = []

    // gamma(..)  with complex numbers (all cases: complete, uncomplete, hypermat)
    // -------------------------------
    if or(type(a)==[1 5]) & ~isreal(a,0)
        if isdef("%s_gamma_user") & type(%s_gamma_user)==13
            y = %s_gamma_user(varargin(:));
            // Note: the overload must be able to process hypermatrices
            return
        else
            error(msprintf(msg, "%s_gamma", "%s_gamma_user()"))
        end
    end
    if a==[] then
        return
    end
    a = real(a)

    // gamma(a) with hypermatrix of real numbers
    // -----------------------------------------
    if rhs==1 then
        y = matrix(gamma(a(:)), sa)
        return
    end

    // gamma(x,a,..) : uncomplete gamma (all cases with real numbers)
    // -------------
    x = varargin(1)
    if type(x)<>1 | ~isreal(x,0) then
        msg = _("%s: Argument #%d: Decimal numbers expected.\n")
        error(msprintf(msg, "gamma", 1))
    end
    if x==[] then
        return
    end
    x = real(x)
    if min(x) <= 0 then
        msg = _("%s: Argument #%d: Must be > %d.\n")
        error(msprintf(msg, "gamma", 1, 0))
    end
    if ~isscalar(x) & ~isscalar(a) & or(size(x)<>size(a)) then
        msg = _("%s: Arguments #%d and #%d: Same sizes expected.\n")
        error(msprintf(msg, "gamma", 1, 2))
    end
    if isscalar(a) then
        a = a * ones(x)
    elseif isscalar(x)
        x = x * ones(a)
    end
    // "upper"
    upper = %f
    u = varargin($)
    if type(u)==10
        if convstr(u(1))<>"upper" then
            msg = _("%s: Argument #%d: ''%s'' expected .\n")
            error(msprintf(msg, "gamma", rhs, "upper"))
        end
        upper = %t
        varargin($) = null()
    end
    // b
    if length(varargin) > 2 then
        b = varargin(3)
        if type(b)<>1 | ~isreal(b,0) then
            msg = _("%s: Argument #%d: Decimal numbers expected.\n")
            error(msprintf(msg, "gamma", 3))
        end
        b = real(b)
        if min(b) <= 0 then
            msg = _("%s: Argument #%d: Must be > %d.\n")
            error(msprintf(msg, "gamma", 3, 0))
        end
        if ~isscalar(a) & ~isscalar(b) & or(size(a)<>size(b)) then
            msg = _("%s: Arguments #%d and #%d: Same sizes expected.\n")
            error(msprintf(msg, "gamma", 2, 3))
        end
    else
        b = 1
    end
    if b==[] then
        return
    end
    if isscalar(b) then
        b = b * ones(a)
    end

    // PROCESSING
    // ==========
    n = ndims(x)
    sa = size(a)
    if n > 2 then
        [x, a, b] = (x(:), a(:), b(:))
    end
    z = find(a==0)
    a(z) = 1

    if upper then
        [?,y] = cdfgam("PQ", x, a, b)
    else
        y = cdfgam("PQ", x, a, b)
    end

    if z<>[] then
        y(z) = 1
    end
    if n > 2 then
        y = matrix(y, sa)
    end
endfunction
