// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 12862 -->
//
// <-- Bugzilla URL -->
//http://bugzilla.scilab.org/12862
//
// <-- Short Description -->
// intsplin() failed for complex ordinates

x = 0.1:0.1:0.5;
assert_checkequal(intsplin((1:5)*%i), 12*%i);
assert_checkequal(intsplin(x, (1:5)*%i), 1.2*%i);
assert_checkequal(intsplin(x, x+(1:5)*%i), 0.12 + 1.2*%i);
