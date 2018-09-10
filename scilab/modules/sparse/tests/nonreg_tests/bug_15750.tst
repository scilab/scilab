// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15750 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15750
//
// <-- Short Description -->
// sparse(0,0) crashes Scilab 6

msg = sprintf(_("%s: Wrong size for input argument #%d: A matrix of size %d x %d expected.\n"), "sparse", 1, 1, 2);

assert_checkerror("sparse(0,0)", msg);
assert_checkerror("sparse(1,1)", msg);

