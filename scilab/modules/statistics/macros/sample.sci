// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003 - INRIA - Carlos Klimann
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

function s = sample(n, X, orient)
    //
    //This function gives a  vector (or matrix) nx1. It contains
    //a random  sample of n extractions,  with replacement, from
    //the matrix X.
    //
    //s = sample(n, X)  (or s = sample(n, X, '*'))  returns a  vector s
    //whose  values are  a random  sample  of n  values from  X,
    //extracted with replacement, from X .
    //
    //s = sample(n, X, 'r')   (or,   equivalently,  s = sample(n, X, 1))
    //returns  a matrix  of type  size(X, 'r')xn.  It  contains a
    //random sample of n  rows, extracted with replacement, from
    //the rows of X.
    //
    //s = sample(n, X, 'c')   (or,   equivalently,  s = sample(n, X, 2))
    //returns  a matrix  of type  nxsize(X, 'c').  It  contains a
    //random  sample of  n columns,  extracted  with replacement
    //from the columns of X.
    //
    [lhs, rhs] = argn(0)
    if rhs < 2 | rhs > 3 then error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "sample", 2, 3)), end
    if X == [] then s = []; return; end
    if rhs == 2 then
        orient = "*"
    end
    if orient == "*" then
        s = X(grand(n, 1, "uin", 1, size(X, "*"))),
        s_size = size(s, '*');
        s = matrix(s, 1, s_size)
        return
    end
    if orient == "r"|orient == 1 then
        s = X(grand(1, n, "uin", 1, size(X, "r")), :)
        return
    end
    if orient == "c"|orient == 2 then
        s = X(:, grand(n, 1, "uin", 1, size(X, "c"))),
        return
    end
    error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', %d or %d expected.\n"), "sample", 3, "*", "r", "c", 1, 2))
endfunction

