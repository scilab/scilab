// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 14347 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14347
//
// <-- Short Description -->
//    plot2d crashed with multiple entries.
// =============================================================================

x=(0.1:0.1:2*%pi)';
errmsg=msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"),"plot2d", 4);
assert_checkerror("plot2d(x, sin(x), cos(x), 2*cos(x))", errmsg);
