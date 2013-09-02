// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 12702 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12702
//
// <-- Short Description -->
// When no extra parameters are needed in the cost function, NDcost does not work:
//
function f = fun(x)
    f = x^2;
endfunction
// Looking for minimum of f(x) = x^2, which is obviously at x = 0 with f(x) = 0.
[f, x] = optim(list(NDcost, fun), 1);
assert_checkalmostequal([f x], [0 0], 0, 1d-20);
