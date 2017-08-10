// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14553 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14553
//
// <-- Short Description -->
//    find(a=b) crashed Scilab.
a = 3;
b = 5;
someStr = "find(a = b)";
assert_checkfalse(execstr(someStr, "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "find", 1, 2);
assert_checkerror(someStr, refMsg);
