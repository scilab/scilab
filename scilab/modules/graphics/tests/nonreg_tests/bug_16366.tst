// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16366 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16366
//
// <-- Short Description -->
// plot(x,y,":") plotted a dashed line instead of a dotted one

clf
plot(0:1,":")
assert_checkequal(gce().children.line_style, 8);
clf
plot(0:1,"g:")
assert_checkequal(gce().children.line_style, 8);
