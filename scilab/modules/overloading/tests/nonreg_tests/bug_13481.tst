// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13481 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13481
//
// <-- Short Description -->
// varn did not work on rational fractions
//
// <-- CLI SHELL MODE -->

// Test on a rational fractions

x = poly(0, "x");
num = 3 + x + x^2;
den = 1 - x + 2*x^2;

R = num / den;
R.dt = 'c';

assert_checkequal("x", varn(R));

num = varn(num, "z");
den = varn(den, "z");

R2 = varn(R, "z");

// Check varn created the same num and den
assert_checkequal(num / den, R2.num / R2.den);

// Check dt is kept with varn
assert_checkequal(R.dt, R2.dt);

// Test on matrix of rationals
matrix_R = [R, 2*R + x, R.den/R.num];
assert_checkequal(varn(matrix_R), "x");
assert_checkequal(varn(varn(matrix_R, "z")), "z");
