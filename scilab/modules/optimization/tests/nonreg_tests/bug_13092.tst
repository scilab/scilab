// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13092 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13092
//
// <-- Short Description -->
// Wrong error message when user function returned a complex number

function d = myf(x)
    mu = 1e6
    eVal_min = 0.4
    A = [4*(x(1)**4 + x(2)**4), 2*x(2)**4; 2*x(2)**4, 4*x(2)**4]
    B = [4*(x(1)**2 + x(2)**2), -3*x(2)**2; -3*x(2)**2, 4*x(2)**2]
    eig = spec(A,B)
    eig = gsort(eig, "g", "i")
    eVal = eig(1)
    d = x(1)**2 + x(2)**2 + mu * (eVal_min - eVal)**2
endfunction

xStart = [1 1];

refMsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 1);
assert_checkerror("[x, fval] = fminsearch(myf, xStart);", refMsg);
