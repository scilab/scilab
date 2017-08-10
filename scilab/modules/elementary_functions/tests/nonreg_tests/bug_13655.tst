//<-- CLI SHELL MODE -->
// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- Non-regression test for bug 13655 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13655
//
// <-- Short Description -->
// acos and asin returned wrong results when input argument was a matrix of size
// greater than 1 and containing at least one NaN value.

assert_checkequal(acos(%nan), %nan);
assert_checkalmostequal(acos(2), 1.3169579*%i, [], 1.e-10);
expected = [1.3169579*%i %nan];
assert_checkalmostequal(acos([2, %nan]), expected, [], 1.e-10);
assert_checkalmostequal(acos([2; %nan]), expected.', [], 1.e-10);
assert_checkalmostequal(acos([0.5 %nan]), [1.0471976 %nan], [], 1.e-7);

assert_checkequal(asin(%nan), %nan);
assert_checkalmostequal(asin(2), %pi/2 - 1.3169579*%i, [], 1.e-10);
expected = [%pi/2 - 1.3169579*%i, %nan];
assert_checkalmostequal(asin([2 %nan]), expected, [], 1.e-10);
assert_checkalmostequal(asin([2 %nan]'), expected.', [], 1.e-10);
