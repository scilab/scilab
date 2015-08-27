//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 13507 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13507
//
// <-- Short Description -->
// Imaginary part in result of a complex power was missing.

R = [1,2;3,4];
c = 1 + 2*%i;

computed = R ^ c;
expected = expm(c*logm(R));
assert_checkfalse(isreal(computed));
assert_checkalmostequal(computed, expected);

c = 2 + 0*%i;
computed = R ^ c;
expected = R ^ 2;
assert_checktrue(isreal(computed));
assert_checkalmostequal(computed, expected);
