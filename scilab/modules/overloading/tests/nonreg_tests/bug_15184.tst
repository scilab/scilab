// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Cedric Delamarre
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15184 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15184
//
// <-- Short Description -->
// comparisons of complex numbers: a = 1 + %i;  a<2  crashes Scilab

a = 1 + %i;
assert_checkerror("a<2", "Complex comparison not supported. Please define %s_1_s_custom() or check your code.");
assert_checkerror("a<=2", "Complex comparison not supported. Please define %s_3_s_custom() or check your code.");
assert_checkerror("a>2", "Complex comparison not supported. Please define %s_2_s_custom() or check your code.");
assert_checkerror("a>=2", "Complex comparison not supported. Please define %s_4_s_custom() or check your code.");
