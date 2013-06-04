// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 -  Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// Run with test_run('statistics', 'NaNTests', ['no_check_error_output'])

nan = %nan;

// Testing that the requested return is %nan when one input argument is %nan

// cdfbet
[res, res2] = cdfbet("PQ", nan, nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
[res, res2] = cdfbet("XY", nan, nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdfbet("A", nan, nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdfbet("B", nan, nan, nan, nan, nan);
assert_checkequal(res, nan);

// cdfbin
[res, res2] = cdfbin("PQ", nan, nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdfbin("S", nan, nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdfbin("Xn", nan, nan, nan, nan, nan);
assert_checkequal(res, nan);
[res, res2] = cdfbin("PrOmpr", nan, nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);

// cdfchi
[res, res2] = cdfchi("PQ", nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdfchi("X", nan, nan, nan);
assert_checkequal(res, nan);
res = cdfchi("Df", nan, nan, nan);
assert_checkequal(res, nan);

// cdfchn
[res, res2] = cdfchn("PQ", nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdfchn("X", nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdfchn("Df", nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdfchn("Pnonc", nan, nan, nan, nan);
assert_checkequal(res, nan);

// cdff
[res, res2] = cdff("PQ", nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdff("F", nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdff("Dfn", nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdff("Dfd",nan, nan, nan, nan);
assert_checkequal(res, nan);

// cdffnc
[res, res2] = cdffnc("PQ", nan, nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdffnc("F", nan, nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdffnc("Dfn", nan, nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdffnc("Dfd", nan, nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdffnc("Pnonc", nan, nan, nan, nan, nan);
assert_checkequal(res, nan);

// cdfgam
[res, res2] = cdfgam("PQ", nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdfgam("X", nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdfgam("Shape", nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdfgam("Rate", nan, nan, nan, nan);
assert_checkequal(res, nan);

// cdfnbn
[res, res2] = cdfnbn("PQ", nan, nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdfnbn("S", nan, nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdfnbn("Xn", nan, nan, nan, nan, nan);
assert_checkequal(res, nan);
[res, res2] = cdfnbn("PrOmpr", nan, nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);

// cdfnor
[res, res2] = cdfnor("PQ", nan, nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdfnor("X", nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdfnor("Mean", nan, nan, nan, nan);
assert_checkequal(res, nan);
res = cdfnor("Std", nan, nan, nan, nan);
assert_checkequal(res, nan);

// cdfpoi
[res, res2] = cdfpoi("PQ", nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdfpoi("S", nan, nan, nan);
assert_checkequal(res, nan);
res = cdfpoi("Xlam", nan, nan, nan);
assert_checkequal(res, nan);

// cdft
[res, res2] = cdft("PQ", nan, nan);
assert_checkequal(res, nan);
assert_checkequal(res2, nan);
res = cdft("T", nan, nan, nan);
assert_checkequal(res, nan);
res = cdft("Df", nan, nan, nan);
assert_checkequal(res, nan);
