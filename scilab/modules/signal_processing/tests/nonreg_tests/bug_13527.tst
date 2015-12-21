//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vladislav TRUBKIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 13527 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13527
//
// <-- Short Description -->
// hilb must be called only with an odd integer number of points
//
lstr = gettext("%s: Wrong type for input argument #%d: An odd integer expected.\n");
errmsg = msprintf (lstr, "hilb", 1);
// KO
assert_checkerror("hilb(2)", errmsg);
assert_checkerror("hilb(14)", errmsg);
assert_checkerror("hilb(26)", errmsg);
assert_checkerror("hilb(26.5)", errmsg);
assert_checkerror("hilb(27.5)", errmsg);
// OK
assert_checkequal(execstr("hilb(3)", "errcatch"), 0);
assert_checkequal(execstr("hilb(7)", "errcatch"), 0);
