// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 4229 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4229
//
// <-- Short Description -->
// delip does not return an error message if one element of first input
// argument is negative

errmsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), "delip", 1, "0", "+INF");
assert_checkerror("delip([-2 2], 0.5)", errmsg);
assert_checkerror("delip([2 -2], 0.5)", errmsg);
