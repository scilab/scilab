// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15481 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15481
//
// <-- Short Description -->
// scatter() and scatter3d() failed for only one point

clf reset
assert_checkequal(execstr("scatter(1,2);", "errcatch"), 0);
assert_checkequal(execstr("scatter(1,2,100);", "errcatch"), 0);
assert_checkequal(execstr("scatter(1,2,100, ""orange"");", "errcatch"), 0);

assert_checkequal(execstr("scatter3d(1,2,3);", "errcatch"), 0);
assert_checkequal(execstr("scatter3d(1,2,3,100);", "errcatch"), 0);
assert_checkequal(execstr("scatter3d(1,2,3,100,""orange"");", "errcatch"), 0);
