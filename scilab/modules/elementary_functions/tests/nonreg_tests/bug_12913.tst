// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12913 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12913
//
// <-- Short Description -->
// linspace errored out if third argument was integer type

y = linspace(0, 5.5, uint32(5.2));
refY = [0 1.375 2.75 4.125 5.5];
assert_checkequal(y, refY);

refMsg = msprintf(_("%s: Wrong value for input argument #%d: An integer value expected.\n"), "linspace", 3);
assert_checkerror("linspace(0, 5.5, 5.2);", refMsg);
