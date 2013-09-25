// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) 2013 - Samuel GOUGEON : processing rewritten, fixing http://bugzilla.scilab.org/5205
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y = permute(x, dims)

    // This function returns an array y which results of the x permutation
    // Input :
    // -x a (multi-dimensionnnal) array of cells, or doubles or strings, ...
    // -dims a vector which contains the permutation order
    // Output :
    // -y the result of the x permutation

    // Verify input arguments number
    if argn(2) <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "permute", 2));
    end

    // Verify if the size of dims corresponds to dimension of x
    if ndims(dims) <> 2 then
        error(msprintf(gettext("%s: Wrong size for argument #%d: Vector expected.\n"), "permute", 2));

    elseif or(gsort(dims,"c","i") <> (1:prod(size(dims)))) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d.\n"), "permute", 2));

    elseif prod(size(dims)) < ndims(x) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: At least the size of input argument #%d expected.\n"), "permute", 2, 1));
    end

    // Case x is empty
    if isempty(x) then
        y = x
        return
    end

    // ---------------- PROCESSING --------------------
    // Existing indices
    s = size(x)
    p = size(x, "*")
    n = 1
    for i = 1:length(s)
        t = "x%d = ones(1,p/(prod(s(1:%d)))) .*. ((1:s(%d)) .*. ones(1,n)) ;"+..
        " n = prod(s(1:%d))\n"
        t = msprintf(t, i, i, i, i)
        execstr(t)
    end
    xlist = strcat(msprintf("x%d\n",(1:length(s))'),",")
    cstr = "sub2ind(s,"+ xlist +")"
    execstr("LI = "+cstr)

    // New indices
    s = s(dims)
    cstr = "sub2ind(s,"+ strcat(msprintf("x%d\n", dims(:)), ",")+")"
    execstr("LI2 = "+cstr)

    // Clearing intermediate memory used
    execstr("clear "+strsubst(xlist, ",", " "))

    // Permutation
    if typeof(x) == "ce"
        y = x
        y.dims = int32(s)
        y(LI2).entries = x(LI).entries
    else
        y(LI2) = x(LI)
        y = matrix(y, s)
    end

endfunction
