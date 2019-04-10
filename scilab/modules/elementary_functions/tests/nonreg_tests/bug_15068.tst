// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15068 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15068
//
// <-- Short Description -->
// size(sum(ones(2,3,4,5) ,4)) was [4 2 3] instead of [2 3 4]

s = sum(ones(2,3,4,5), 4);
assert_checkequal(size(s), [2 3 4]);
assert_checkequal(s, ones(2,3,4)*5);
