// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function ret = isRelease()
    [a,b] = getversion();
    idx = find(b == "release");
    ret = %t;

    if isempty(idx) then
        ret = %f;
    end
endfunction
