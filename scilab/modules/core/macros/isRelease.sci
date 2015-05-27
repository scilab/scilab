// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function ret = isRelease()
    [a,b] = getversion();
    if or(getscilabmode() == ["NWNI", "SCILAB_API"]) then
        ret = b(4) == "release";
    else
        ret = b(5) == "release";
    end
endfunction
