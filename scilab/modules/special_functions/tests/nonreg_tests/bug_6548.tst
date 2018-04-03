// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 6548 -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6548
// <-- Short Description -->
// gamma() could not accept an hypermatrix
//
// <-- Non-regression test for bug 10450 (complement) -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10450
// <-- Short Description -->
// gamma() could not be overloaded for complex numbers

// hypermatrix of decimal numbers
a = grand(2, 2 , 2, "uin",2,10);
assert_checkequal(gamma(a), matrix(factorial(a(:)-1), size(a)));

// complex numbers
// matrix
c = [1-%i 1+%i];
expected = ["%s_gamma: Function not defined for the given argument type."
            "  Check arguments or define function %s_gamma_user() for overloading."]
assert_checkerror("gamma(c)", expected);

// hypermatrix of complex numbers
c = a*(1-%i);
assert_checkerror("gamma(c)", expected);
