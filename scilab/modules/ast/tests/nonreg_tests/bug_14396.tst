// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14396 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14396
//
// <-- Short Description -->
// Display of real number with exponent was wrong

format("v");
format(10);

a = 1.e-10;
disp(a) // for dia.ref
str = "1.000D-10";
assert_checkequal(string(a), str);

format("e");
format(10);

a = 0;
str = "0.000D+00"; // 9 characters + sign
disp(0); // for dia.ref
assert_checkequal(string(a), str);

format("v");
format(18);
a = 12345678900000012;
disp(a) // for dia.ref
str = "1.23456789000D+16";

assert_checkequal(string(a), str);
