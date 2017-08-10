// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->   // the test is not about the display
//
// <-- Non-regression test for bug 13192 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13192
//
// <-- Short Description -->
// horner(rationals_column, rationals_row) sometimes failed

s = %s;
H = [1/s; 1/s; 1/(s+1)];
f = [1+%i 1/(1-s)];
expectedN = [ 1 1-%s ; 1 1-%s ; 1 %s-1];
expectedD = [ 1+%i 1 ; 1+%i 1 ; 2+%i %s-2];
assert_checkequal(horner(H,f), expectedN./expectedD);
