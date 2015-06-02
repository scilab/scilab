// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13735 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13735
//
// <-- Short Description -->
// There was not check on frameflag value in plot2d.

x = 1:10;
y = 0 * x;
assert_checkerror("plot2d(x, y, 5, frameflag = -4)", _("%s: Wrong value for frameflag option.\n"), [], "plot2d");
assert_checkerror("plot2d(x, y, 5, axesflag = -4)", _("%s: Wrong value for axesflag option.\n"), [], "plot2d");