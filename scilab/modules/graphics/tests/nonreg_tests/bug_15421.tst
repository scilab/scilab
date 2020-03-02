// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15421 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15421
//
// <-- Short Description -->
// The default grid_style value is now 7

assert_checkequal(gda().grid_style, [7 7]);
clf reset
param3d([0 1],[0 1],[0 1]);
assert_checkequal(gca().grid_style, [7 7 7]);
