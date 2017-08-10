// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 9161 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9161
//
// <-- Short Description -->
//  Wrong insertions in sparse when repeated index

A = sparse(33);
B = sparse([1 0 3]');
ind = [1 1 1];
A(ind) = B;
assert_checkequal(A, sparse(3));

A = sparse([33 -10]);
ind = [2 1 2];
A(ind) = B;
assert_checkequal(A, sparse([0 3]));
