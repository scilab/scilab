// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14386 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14386
//
// <-- Short Description -->
// Creating a string from a macros generates invalid code.


function test()
    if 2 > 1 then
        disp("Bug 14386 fixed")
    end
endfunction
[i,o,t] = string(test);
assert_checkequal(t($-1), "end");
