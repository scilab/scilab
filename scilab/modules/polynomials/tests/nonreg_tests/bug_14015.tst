// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14015 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14015
//
// <-- Short Description -->
// Nany Nan term added to a polynomial was ignored

assert_checkequal(coeff(%nan+%z), [%nan 1]);
assert_checkequal(coeff(1+%nan*%z), [%nan %nan]);
assert_checkequal(coeff(%nan+%nan*%z), [%nan %nan]);
assert_checkequal(coeff(%nan+0*%z), %nan);
assert_checkequal(coeff(%nan*%s^2 + %s^4), [%nan %nan %nan 0 1]);
