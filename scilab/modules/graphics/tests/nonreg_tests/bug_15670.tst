// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15670 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15670
//
// <-- Short Description -->
// set(H, prop1, val1, prop2, val2,..) : setting multiple properties for a
// vector of handles yielded an error.

plot2d()
e = gce().children;
set(e, "line_mode","on", "thickness", 2);
assert_checkequal(e.line_mode, ["on" "on" "on"]');
assert_checkequal(e.thickness, [2;2;2]);
