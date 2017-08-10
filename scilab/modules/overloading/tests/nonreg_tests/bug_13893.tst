//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 13893 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13893
//
// <-- Short Description -->
// simp() function does not set a rational denominator at 1 when numerator is equal to zero

r = rlist(0,1+2*%z^2);
r_res = simp(r);
assert_checkequal(numer(r_res), 0);
assert_checkequal(denom(r_res), 1+0*%z);
