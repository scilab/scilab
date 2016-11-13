// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 14429 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14429
//
// <-- Short Description -->
// With a rational r in simp_mode(%t), r-r and r+(-r) dit not simplify the denominator to 1

sm = simp_mode();
simp_mode(%t);
r = 1/(%i-%z)^3;
s = r-r;
assert_checkequal(s.den, poly(1,"z","coeff"));
a = r+(-r);
assert_checkequal(a.den, poly(1,"z","coeff"));
simp_mode(sm);
