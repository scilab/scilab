// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 -  Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// Run with test_run('statistics', 'InfTests', ['no_check_error_output'])

inf = %inf;
tol = 1e-299;

// Testing that the requested return is %inf when one input argument is %inf

// cdfchi
[res, res2] = cdfchi("PQ", inf, 1);
assert_checkequal(res, 1);
assert_checkequal(res2, 0);
[res, res2] = cdfchi("PQ", 1, inf);
assert_checkalmostequal(res, 0, tol);
assert_checkalmostequal(res2, 1, tol);

// cdfchn
[res, res2] = cdfchn("PQ", inf, 1, 1);
assert_checkequal(res, 1);
assert_checkequal(res2, 0);
[res, res2] = cdfchn("PQ", 1, inf, 1e-10);
assert_checkalmostequal(res, 0, tol);
assert_checkalmostequal(res2, 1, tol);

// cdff
[res, res2] = cdff("PQ", inf, 1, 1e-50);
assert_checkequal(res, 1);
assert_checkequal(res2, 0);
[res, res2] = cdff("PQ", 1, inf, 1e-50);
assert_checkalmostequal(res, 1, tol);
assert_checkalmostequal(res2, 0, tol);

// cdffnc
[res, res2] = cdffnc("PQ", inf, 1, 10, 0);
assert_checkequal(res, 1);
assert_checkequal(res2, 0);
[res, res2] = cdffnc("PQ", 1e-10, inf, 10, 0);
assert_checkalmostequal(res, 0, tol);
assert_checkalmostequal(res2, 1, tol);

// cdfgam
[res, res2] = cdfgam("PQ", inf, 1, 1);
assert_checkequal(res, 1);
assert_checkequal(res2, 0);
[res, res2] = cdfgam("PQ", 1, inf, 1);
assert_checkalmostequal(res, 0, tol);
assert_checkalmostequal(res2, 1, tol);
[res, res2] = cdfgam("PQ", 1, 1, inf);
assert_checkalmostequal(res, 1, tol);
assert_checkalmostequal(res2, 0, tol);

// cdfnbn
[res, res2] = cdfnbn("PQ", inf, 1, 0, 1);
assert_checkequal(res, 1);
assert_checkequal(res2, 0);
[res, res2] = cdfnbn("PQ", 1, inf, 0, 1);
assert_checkalmostequal(res, 0, tol);
assert_checkalmostequal(res2, 1, tol);

// cdfnor
[res, res2] = cdfnor("PQ", inf, 1, 1);
assert_checkequal(res, 1);
assert_checkequal(res2, 0);
[res, res2] = cdfnor("PQ", -inf, 1, 1);
assert_checkequal(res, 0);
assert_checkequal(res2, 1);
[res, res2] = cdfnor("PQ", 1, inf, 1);
assert_checkalmostequal(res, 0, tol);
assert_checkalmostequal(res2, 1, tol);
[res, res2] = cdfnor("PQ", 1, -inf, 1);
assert_checkalmostequal(res, 1, tol);
assert_checkalmostequal(res2, 0, tol);

// cdfpoi
[res, res2] = cdfpoi("PQ", inf, 1);
assert_checkequal(res, 1);
assert_checkequal(res2, 0);
[res, res2] = cdfpoi("PQ", 1, inf);
assert_checkalmostequal(res, 0, tol);
assert_checkalmostequal(res2, 1, tol);

// cdft
[res, res2] = cdft("PQ", inf, 1);
assert_checkequal(res, 1);
assert_checkequal(res2, 0);
[res, res2] = cdft("PQ", -inf, 1);
assert_checkequal(res, 0);
assert_checkequal(res2, 1);
