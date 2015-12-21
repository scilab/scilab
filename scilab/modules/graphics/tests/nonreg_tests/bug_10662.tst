// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//
// <-- Non-regression test for bug 10662 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10662
//
// <-- Short Description -->
// xload / load_user_data() does not support tlist


// SOD save/load
plot2d();
a = gca();
set(a, "user_data", tlist(["test" "a"], %pi));
f = gcf();
save("TMPDIR/test.scg", "f");
delete(f);
load("TMPDIR/test.scg");
a = gca();
assert_checkequal(a.user_data, tlist(["test" "a"], %pi));
delete(gcf());
