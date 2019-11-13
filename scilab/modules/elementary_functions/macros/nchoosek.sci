// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2009 - John Burkardt
// Copyright (C) 2009 - 2010 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - Michael Baudin
// Copyright (C) 2019 - Samuel GOUGEON - Le Mans Université
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = nchoosek(n, k, logFormat)

    //   Returns the binomial number (n,k) and/or its log10.
    //
    // Parameters
    //   n : a matrix of floating point integers, must be positive
    //   k : a matrix of floating point integers, must be positive
    //
    // Syntax
    //   b = nchoosek(n, k)
    //   [logb, b] = nchoosek(n, k)
    //   [logb, b] = nchoosek(n, k, logFormat)
    //   logb = nchoosek(n, k, logFormat)

    fname = "nchoosek"

    // CHECKING ARGUMENTS
    // ==================
    // Number of input argument
    [lhs, rhs] = argn()
    if rhs < 2
        msg = _("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, fname, 2, 3))
    end
    // Type and value of n
    msg1 = _("%s: Argument #%d: Decimal integer expected.\n")
    msg2 = _("%s: Argument #%d: Non-negative integers expected.\n")
    if type(n) <> 1 | or(round(n)<>n) then
        error(msprintf(msg1, fname, 1))
    end
    if or(n<0)
        error(msprintf(msg2, fname, 1))
    end
    // Type and value of k
    if type(k) <> 1 | or(round(k)<>k) then
        error(msprintf(msg1, fname, 2))
    end
    if or(k<0)
        error(msprintf(msg2, fname, 2))
    end
    if length(n)>1 & length(k)>1 & or(size(n)<>size(k)) then
        msg = _("%s: Arguments #%d and #%d: Incompatible sizes.\n")
        error(msprintf(msg, fname, 1, 2))
    end

    i = find(n<k, 1)
    if i <> [] then
        msg = _("%s: n(%d) < k(%d) is forbidden.\n")
        error(msprintf(msg, fname, i, i))
    end

    if length(k)==1 then
        k = ones(n) * k
    elseif length(n)==1
        n = ones(k) * n
    end
    // logFormat
    if isdef("logFormat","l") then
        if type(logFormat) <> 10
            msg = _("%s: Argument #%d: Text expected.\n")
            error(msprintf(msg, fname, 3))
        end
        logFormat = convstr(logFormat(1))
        if and(logFormat<>["log10" "10.mant"])
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, fname, 3, "''log10'',''10.mant''"))
        end
    end

    // PROCESSING
    // ==========
    b = ones(n)
    i = find(n>k)
    if (i<>[]) then
        b(i) = 1 ./ ( (n(i)-k(i)) .* beta(k(i)+1, n(i)-k(i)) )
    end
    b(k==1) = n(k==1)  // C(n,1) = n
    b(n==k) = 1        // C(n,n) = 1
    b = round ( b )

    if argn(1)>1 | isdef("logFormat","l")then
        p = isinf(b)
        logb = b;
        logb(~p) = log10(b(~p));
        if or(p)
            logb(p) = (gammaln(n(p)+1) - gammaln(k(p)+1) - gammaln(n(p)-k(p))  - log(n(p)-k(p)))/log(10)
        end
        if isdef("logFormat","l") & logFormat=="10.mant" then
            logb = int(logb) + 10^(logb-int(logb)-1)
        end
        logb(logb<0) = %nan // Too big n. Example: [c, l] = nchoosek(1e110,2)
    end

    // OUTPUT
    // ======
    //   b = nchoosek(n, k)
    //   [logb, b] = nchoosek(n, k)
    //   [logb, b] = nchoosek(n, k, logFormat)
    //   logb = nchoosek(n, k, logFormat)
    varargout = list()
    if lhs==2 | isdef("logFormat","l") then
        varargout(1) = logb
    end
    if lhs==1 & ~isdef("logFormat","l") | lhs==2 then
        varargout($+1) = b
    end
endfunction
