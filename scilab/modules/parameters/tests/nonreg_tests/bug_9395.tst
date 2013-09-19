// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9395 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9395
//
// <-- Short Description -->
// add_params() did not check its input arguments.

p = init_param();
refMsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"), "add_param", 3);
assert_checkerror("p = add_param();", refMsg);
assert_checkerror("p = add_param(p);", refMsg);
assert_checkerror("p = add_param(p, ''field1'');", refMsg);
