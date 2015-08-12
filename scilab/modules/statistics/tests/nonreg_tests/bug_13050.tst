// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 13050 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13050
//
// <-- Short Description -->
//    The result of mvvacov was not symmetric.
// =============================================================================

x = 1.e8 *  rand(100, 3);
M = mvvacov(x);
[m, n] = size(M);
assert_checkequal(M-M', zeros(m, n));
