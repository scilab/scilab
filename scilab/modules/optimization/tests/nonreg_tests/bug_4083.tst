// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 4083 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4083
//
// <-- Short Description -->
//    The numderivative function is created, which replaces both derivative and
//    numdiff functions.
// =============================================================================

function y = myfunction(x)
    y = x^3;
endfunction

x = 1;
g = numdiff(myfunction, x);
expectedJ = 3;
assert_checkalmostequal(g, expectedJ, [], 1.d-8);

[J, H] = derivative(myfunction, x);
expectedH = 6;
assert_checkalmostequal(J, expectedJ, [], 1.d-8);
assert_checkequal(H, expectedH);

[J,H] = numderivative(myfunction, x)
assert_checkalmostequal(J, expectedJ, [], 1.d-8);
assert_checkequal(H, expectedH);
