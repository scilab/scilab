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
// <-- Non-regression test for bug 16452 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16452
//
// <-- Short Description -->
// setdiff(sparse([1 3 0 2]), sparse([3 7])) missed 0 and listed 3

r = setdiff(sparse([1 3 0 2]), sparse([3 7]));
assert_checkequal(r, sparse([0 1 2]));
