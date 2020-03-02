// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 8930 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8930
//
// <-- Short Description -->
// Some assignments in boolean sparse matrices failed


A = sparse(%t);
A(:) = %t;
assert_checktrue(A);
A = sparse(%t);
A(:) = %f;
assert_checkfalse(A);
A = sparse([%t %f]);
A(:) = %t;
assert_checkequal(A, sparse([%T %T]));
A(:) = %f;
assert_checkequal(A, sparse([%F %F]));
