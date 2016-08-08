// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 8493 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8493
//
// <-- Short Description -->
// Dividing p1./p2 with p1 or/and p2 complex-encoded polynomials with null
// real or imaginary parts for both p1 and p2 prevented some simplifications

z = %z;
// With scalar polynomials
assert_checkequal(((1+0*%i)*z^2+z^3+z^4) ./ z^2, rlist(1+z+z^2, 1));
assert_checkequal(((1+0*%i)*z^2+z^3+z^4) / z^2, rlist(1+z+z^2, 1));
assert_checkequal((%i*z^2+%i*z^3+%i*z^4) ./ (%i*z^2), rlist(1+z+z^2, 1));
assert_checkequal((%i*z^2+%i*z^3+%i*z^4) / (%i*z^2), rlist(1+z+z^2, 1));

// With matrices of polynomials
p1 = ((1+0*%i)*z^2+z^3+z^4); p1 = [p1 p1];
r = rlist(1+z+z^2, 1); r = [r r];
assert_checkequal(p1 ./ z^2, r);
p1 = (%i*z^2+%i*z^3+%i*z^4) ; p1 = [p1 p1];
assert_checkequal(p1 ./ (%i*z^2), r);
