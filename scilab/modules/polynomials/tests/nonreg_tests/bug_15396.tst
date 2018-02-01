// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15396 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15396
//
// <-- Short Description -->
// [m,n,p] = size(hr) yielded an error for an hypermatrix hr of rationals

o = ones(5,4,3,2);
r = rlist(o, o*%z)

assert_checkequal(size(r), [5 4 3 2]);
assert_checkequal(size(r, "*"), 120);
assert_checkequal(size(r, 1), 5);
assert_checkequal(size(r, "r"), 5);
assert_checkequal(size(r, 2), 4);
assert_checkequal(size(r, "c"), 4);
assert_checkequal(size(r, 3), 3);
assert_checkequal(size(r, 4), 2);

[m, n] = size(r);
assert_checkequal([m n], [5 4]);
[m, n, p] = size(r);
assert_checkequal([m n p], [5 4 3]);
[m, n, p, q] = size(r);
assert_checkequal([m n p q], [5 4 3 2]);
