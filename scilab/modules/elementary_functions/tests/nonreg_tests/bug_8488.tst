// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 8488 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8488
//
// <-- Short Description -->
// The min and max function inconsistently propagate NaN inputs

// min
assert_checkfalse(isnan(min([1 %nan])));
assert_checkfalse(isnan(min([%nan 1])));
assert_checktrue(isnan(min([%nan %nan])));

A = rand(3,3);
A(2) = %nan;

B = rand(3,3);
B(6) = %nan;

M = min(A, B);

assert_checkfalse(isnan(M));

// max
assert_checkfalse(isnan(max([1 %nan])));
assert_checkfalse(isnan(max([%nan 1])));
assert_checktrue(isnan(max([%nan %nan])));

A = rand(3,3);
A(2) = %nan;

B = rand(3,3);
B(6) = %nan;

M = max(A, B);

assert_checkfalse(isnan(M));
