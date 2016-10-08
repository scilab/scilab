// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14801 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14801
//
// <-- Short Description -->
// The horizontal concatenation of cells arrays misworked

a = {[1;-0.5],[]; 0.3, 1};
b = {"test"; %z};
r = [a b];
assert_checkequal(r{1,2}, a{1,2});
assert_checkequal(r{2,1}, a{2,1});
assert_checkequal(r{1,3}, b{1,1});

A = cat(3,a,a);
B = cat(3,b,b);
R = [A B];
assert_checkequal(R{1,2,2}, a{1,2});
assert_checkequal(R{2,1,2}, a{2,1});
assert_checkequal(R{1,3,2}, b{1,1});
