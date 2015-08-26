// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9311 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9311
//
// <-- Short Description -->
// The size function returned an error when a dimension out of the order is claimed.

A = 1;
assert_checkequal(size(A, 3), 1);

[m, n, p] = size(A);
assert_checkequal(m, 1);
assert_checkequal(n, 1);
assert_checkequal(p, 1);

A = matrix(1:2, 1, 1, 2);
assert_checkequal(size(A, 4), 1);