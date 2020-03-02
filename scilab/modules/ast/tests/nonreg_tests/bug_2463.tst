// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 2463 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/2463
//
// <-- Short Description -->
// Named arguments were accepted in built-in functions

a = 2;
msg = "type: Wrong number of input arguments: 1 expected.";
assert_checkerror("type(a=%t)", msg);

a = 2;
msg = "sin: Wrong number of input argument(s): 1 expected.";
assert_checkerror("sin(a=0)", msg);
