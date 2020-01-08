// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14610 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14610
//
// <-- Short Description -->
// x = riccati(h,e) yielded an error

h = rand(2,2);
e = rand(2,2);
assert_checkequal(execstr("x = riccati(h, e)", "errcatch"), 0);
