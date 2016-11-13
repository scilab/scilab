// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 14461 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14461
//
// <-- Short Description -->
//    Calling grand(n, "markov", P, x0) did not return all outputs.
// =============================================================================

n = 200;
P = [0.1 0.3 0.2 0.01 0.09 0.3 ; ...
0.2 0.1 0.1 0.5  0.05 0.05; ...
0   0   0.2 0.3  0.4  0.1 ; ...
0.4 0   0.1 0.05 0.05 0.4 ; ...
0.1 0.1 0.1 0.3  0.2  0.2 ; ...
0.2 0.2 0.1 0.1  0.2  0.2];
x0 = [1:3];

a=grand(n, "markov", P, x0);
assert_checkequal(size(a), [3 200]);
