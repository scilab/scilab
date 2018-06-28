// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 6939 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6939
//
// <-- Short Description -->
// nearfloat("succ",rand(2,2,2)) returns a (2,2) matrix with no error

x = rand(8, 1);
xs = nearfloat("succ", x);
xp = nearfloat("pred", x);
assert_checkequal(nearfloat("succ", matrix(x, 2, 2, 2)), matrix(xs, 2, 2, 2));
assert_checkequal(nearfloat("pred", matrix(x, 2, 2, 2)), matrix(xp, 2, 2, 2));
msg = sprintf(_("%s: Wrong type for input argument #%d : A real matrix expected.\n"), "nearfloat", 2); 
assert_checkerror("nearfloat(""succ"", %i)", msg);
