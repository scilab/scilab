// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 12123 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12123
//
// <-- Short Description -->
//   eigs function: incorrect test and error message
// =============================================================================

A = speye(10,10);
B = speye(4,4);
assert_checkerror("eigs(A, B)",[],10000);

