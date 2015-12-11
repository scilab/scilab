// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Anais AUBERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13773 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13773
//
// <-- Short Description -->
// save with negative second argument dit not trigger an error


errmsg = msprintf(_("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), "size", 2);
assert_checkerror("size(ones(2,3),0)", errmsg);
assert_checkerror("size(ones(2,3),-1)", errmsg);
assert_checkerror("size(ones(2,3),-2)", errmsg);
