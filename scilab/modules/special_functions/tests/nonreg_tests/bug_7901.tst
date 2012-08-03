// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7901 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7901
//
// besselh.tst failed on Windows

alpha = 0;
x = 0;
yref = [%nan + %nan * %i];
y = besselh(alpha , x);
assert_checkequal(isreal(y), %F);
assert_checkequal(imag(y), %nan);
assert_checkequal(real(y), %nan);
