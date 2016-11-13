// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) 2013 - Samuel GOUGEON : processing rewritten, fixing http://bugzilla.scilab.org/5205
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be a valid permutation vector.\n"), "permute", 2));

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

    // Treat extra dimensions for permutations as 1
    s = [s ones(1, length(dims) - length(s))];

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
    if typeof(x) == "ce" then
        y = cell(s);
        for i=1:size(LI2,2)
            y{LI2(i)} = x{LI(i)};
        end
    else
        y(LI2) = x(LI)
        y = matrix(y, s)
    end

endfunction
