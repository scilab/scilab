// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14591 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14591
//
// <-- Short Description -->
// <= and >= operators comparing 2 hypermatrices of decimal numbers or encoded
//  integers were inverted

a = ones(2,2,2);
r = ones(2,2,2)>0;
assert_checkequal(a<=2*a, r);
assert_checkequal(2*a>=a, r);
assert_checkequal(int8(a)<=int8(2*a), r);
assert_checkequal(int8(2*a)>=int8(a), r);
