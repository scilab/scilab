// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13511 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13511
//
// <-- Short Description -->
// lcm used with doubles returned wrong type values,
// bezout help did not mention doubles.

assert_checkequal(lcm([96 6250 10000 18700]), int32(56100000));

[P, U] = bezout(3.5, 4.2);
assert_checkequal(coeff(P), 1);
assert_checkalmostequal(coeff(U), [0 4.2; 1/4.2 -3.5]);
