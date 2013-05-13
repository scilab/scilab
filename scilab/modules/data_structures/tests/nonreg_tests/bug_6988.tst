// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6988 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6988
//
// <-- Short Description -->
// Error messages in modules/data_structures/src/c/hmops.c are not standard.

M = hypermat([2 3 2 2],1:24)
errmsg1=msprintf(_("%s: Wrong number of input arguments: at most %d expected.\n"),"hmops",4);
assert_checkerror("M(:,:,1,2,3)",errmsg1);
