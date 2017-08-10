// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2011 - DIGITEO - Michael Baudin
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
function [M, sel] = pack(M, blocksizes)
    //
    // Check input arguments
    [lhs, rhs] = argn();
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"pack",2));
    end
    //
    // Check type
    if and(typeof(M) <> ["constant" "sparse"]) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real matrix or sparse matrix expected.\n"),"pack",1));
    end
    if typeof(blocksizes) <> "constant" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"),"pack",2));
    end
    //
    // Check content
    if ~isreal(M) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"),"pack",1));
    end
    if ~isreal(blocksizes) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"),"pack",2));
    end
    if or(blocksizes<1) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Non-negative integers expected.\n"),"pack",2));
    end
    if or(blocksizes<>floor(blocksizes)) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer value expected.\n"),"pack",2));
    end
    //
    // Proceed...
    sel = []
    kk = 0
    blocksizes = matrix(blocksizes, 1, size(blocksizes,"*"))
    for ni=blocksizes
        k = kk
        for j=1:ni
            sel = [sel, k+(j:ni)]
            k = k+ni
        end
        kk = kk+ni*ni
    end
    M = M(sel, :)

endfunction
