// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 12747 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12747
//
// <-- Short Description -->
//    The legendre function did not manage the -1 and 1 values for the third 
//    argument.
// =============================================================================

res = legendre(2,2,-1);
assert_checkequal(res, 0);

res = legendre(0, 0:2, [1 -1]);
expected = [1 1;0 0;0 0]
assert_checkequal(res, expected);
