// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14113 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14113
//
// <-- Short Description -->
// infinite loop is not detected

old = recursionlimit(20);
function f()
    f();
endfunction

msg = msprintf(_("Recursion limit reached (%d).\n"), recursionlimit());
assert_checkerror("f()", msg);

recursionlimit(old);
