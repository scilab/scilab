// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
