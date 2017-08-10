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

function s = samplef(n, X, f, orient)
    //
    //This function gives s, a  vector of length n.  It contains
    //a  sample of  n  extractions, with  replacement, from  the
    //vector  (or  matrix)  X,  each element  counted  with  the
    //frequency given by the corresponding value in vector f.
    //
    //s = samplef(n, X, f)   (or  s = samplef(n, X, f, '*'))   returns  a
    //vector s whose values are a random sample of n values from
    //X,   each  value   with  a   probability  to   be  sampled
    //proportional  to the corresponding  value of  f, extracted
    //with replacement, from X.  f must have same length than X.
    //
    //s = samplef(n, X, f, 'r')           (or,          equivalently,
    //s = samplef(n, X, f, 1))    returns    a    matrix   of    type
    //size(X, 'r')xn.  It contains a random sample of n rows from
    //X, each row with  a probability to be sampled proportional
    //to   the  corresponding   value  of   f,   extracted  with
    //replacement, from the  rows of X. The length  of f must be
    //equal to the number of rows of X.
    //
    //s = samplef(n, X, f, 'c')           (or,          equivalently,
    //s = samplef(n, X, f, 2))    returns    a    matrix   of    type
    //nxsize(X, 'c').  It  contains a random sample  of n columns
    //from  X, each  column  with a  probability  to be  sampled
    //proportional  to the corresponding  value of  f, extracted
    //with replacement, from the columns  of X.  The length of f
    //must be equal to the number of columns of X.
    //
    [lhs, rhs] = argn(0)
    if rhs < 3 | rhs > 4 then error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "samplef", 3, 4)), end
    if X == [] then s = []; return; end

    // Frequencies cannot be negative
    if ~and(f >= 0)
        error(msprintf(_("%s: Wrong value for input argument #%d: Non-negative values expected.\n"), fname, 3))
    end

    // Setting default value for 4th argument
    if rhs == 3 then
        orient = "*"
    end

    sizx = size(X)

    if orient == "*" then
        if size(f, "*") <> prod(sizx) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d and #%d: Same size expected.\n"), "samplef", 2, 3)),
        end
        //Strip X of 0 frequency values
        X(find(f==0)) = [];
        f(find(f==0)) = [];
        val = cumsum([0; f(:)])
        y = grand(n, 1, "unf", 0, val($))
        ind = dsearch(y, val, "c")
        s = X(ind)
        s_size = size(s, '*')
        s = matrix(s, 1, s_size) //consistently returns s in a line vector when called without row or column orientation;
        return
    end

    if orient == "r"|orient == 1 then
        if size(f, "*") <> sizx(1) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same number of rows of the second input argument expected.\n"), "samplef", 3)),
        end
        //Strip X of 0 frequency values
        X(find(f==0), :) = [];
        f(find(f==0)) = [];
        val = cumsum([0; f(:)])
        y = grand(n, 1, "unf", 0, val($))
        ind = dsearch(y, val, "c")
        s = X(ind, :)
        return
    end

    if orient == "c"|orient == 2 then
        if size(f, "*") <> sizx(2) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same number of columns of the second input argument expected.\n"), "samplef", 3)),
        end
        //Strip X of 0 frequency values
        X(:, find(f==0)) = [];
        f(find(f==0)) = [];
        val = cumsum([0; f(:)])
        y = grand(n, 1, "unf", 0, val($))
        ind = dsearch(y, val, "c")
        s = X(:, ind)
        return
    end

    error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', %d or %d expected.\n"), "orient", 4, "*", "r", "c", 1, 2))
endfunction

