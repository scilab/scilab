// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Farid BELAHCENE
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = bitcmp(x, n)

    // BITCMP function: bitwise complement of integers
    //
    // P. Marechal, 2008 : Add argument check
    // S. Gougeon, 2020 :
    //   * Entension to 64 bit integers.
    //   * Extension to all signed integers.
    //   * Decimal positive integers > 2^52 up to 2^1024 = number_properties("huge")
    //      can now be processed, with bitnum up to 1024 instead of 52.
    //   * bitnum is now optional as well for input decimal integers.
    //     It can actually be an array.

    rhs = argn(2);

    // CHECK INPUT ARGUMENTS
    // =========================================================================
    if rhs < 1 | rhs > 2 then
        msg = gettext("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, "bitcmp", 1, 2));
    end


    // Check types
    if and(type(x)<>[1 8]) | (type(x)==1 & ~isreal(x))
        msg = gettext("%s: Argument #%d: Decimal or encoded integers expected.\n");
        error(msprintf(msg, "bitcmp",1))
    end
    if x==[] then
        y = []
        return
    end
    if type(x)==1 & (or(x-floor(x)<>0) | or(x<0)) then
        msg = gettext("%s: Argument #%d: Positive decimal integers expected.\n");
        error(msprintf(msg, "bitcmp",1))
    end
    select inttype(x)
    case 0  then
        nmax = 1024
        p = int(log2(x)) + 1
    case 1 then nmax = 8
    case 2 then nmax = 16
    case 4 then nmax = 32
    case 8 then nmax = 64
    case 11 then nmax = 8
    case 12 then nmax = 16
    case 14 then nmax = 32
    case 18 then nmax = 64
    end

    if rhs == 2
        if size(n,"*")<>1 & or(size(n)<>size(x))
            msg = gettext("%s: Argument #%d: Wrong size.\n")
            error(msprintf(msg, "bitcmp", 2))
        end
        if and(type(n)<>[1 8]) | n<>int(n) | (type(n)==1 & ~isreal(n))
            msg = gettext("%s: Argument #%d: Real or encoded integer expected.\n")
            error(msprintf(msg, "bitcmp", 2))
        end
        if n > nmax | n<1 then
            msg = gettext("%s: Argument #%d: Must be in the interval [%d, %d].\n")
            error(msprintf(msg, "bitcmp", 2, 1, nmax));
        end
    else
        if type(x)==8
            n = nmax
        else
            n = ones(x)*53
            k = abs(x) > 1/%eps
            n(k) = p(k)
        end
    end

    // PROCESSING
    // =========================================================================
    // Encoded integers
    if type(x)==8 then
        y = ~x
        // Canceling bits > n:
        if rhs > 1 then
            it = inttype(x)
            y = y & (iconvert(2,it)^n - 1)
        end

    else
    // Decimal integers
        y = x
        mask = 2.^n - (2.^max(0,n-52))
        k = n < p
        y(k) = bitand(y(k), mask)
        y = bitxor(y, mask)
    end

endfunction
