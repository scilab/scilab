// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14082 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14082
//
// <-- Short Description -->
// m=1; m()=1; makes Scilab crash.

m = 1;
errmsg = msprintf(gettext("Wrong insertion : Cannot insert without arguments."));
assert_checkerror("m()=1", errmsg);
assert_checkerror("m()=""string""", errmsg);

m = "b";
assert_checkerror("m()=1", errmsg);
assert_checkerror("m()=""string""", errmsg);
