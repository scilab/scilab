// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12706 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12706
//
// <-- Short Description -->
// A wrong size matrix as input argument is not detected

A = rand(2,3);
errmsg = msprintf(_("%s: Wrong size for input argument #%d: A matrix of size 1x3 or 3xN expected.\n"), "cross", 1);
assert_checkerror("cross(A,A)", errmsg);
