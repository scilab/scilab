// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function x = %s_y_s(b, a)
    //  x = b ./. a  such that b = x .*. a
    // a, b: arrays of any sizes, including hypermatrices

    // size(a) = (p, q, ..)
    // size(x) = (m, n, ..)
    // size(b) = (m*p, n*q, ..)

    // Extended size of a:
    sa = [size(a) ones(1, ndims(b)-ndims(a))];  // [p, q,..]

    // Size of x
    sx = size(b) ./ sa;
    if or(sx <> int(sx)) then
        msg = _("%s: Arguments #%d and #%d: Incompatible sizes.\n")
        error(msprintf(msg, "./. :", 1, 2))
    end
        // for ones(Sx(:)):
    Sx = list(); for s = sx, Sx($+1) = s, end

    // Reordering b components to allow a matricial processing:
    ia = matrix(1:prod(sa), sa);
    tmp = ones(Sx(:)) .*. ia;
    [tmp, k] = gsort(tmp,"g","i");
    b = matrix(b(k), prod(sx), prod(sa))

    // Least square Processing
    x = b / a(:).'

    // Reshaping the raw result
    x = matrix(x, sx)
endfunction
