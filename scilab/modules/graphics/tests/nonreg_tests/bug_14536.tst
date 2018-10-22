// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 14536 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14536
//
// <-- Short Description -->
// With negative coordinates in logarithmic mode, xpoly() crashed Scilab

ax = gca();
ax.data_bounds = [0.1 0;100 1];
ax.log_flags = "lnn";
msg = "Error: Values must be strictly positive when logarithmic mode on x axis is active."

cmd = "xpoly([0;0],[0;1])";
assert_checktrue(assert_checkerror(cmd, msg));

cmd = "xpoly([-1;0],[0;1])";
assert_checktrue(assert_checkerror(cmd, msg));
