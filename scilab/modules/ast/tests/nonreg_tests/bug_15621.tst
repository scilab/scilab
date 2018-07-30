// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 15621 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15621
//
// <-- Short Description -->
// polynomial exponentiation silently truncates exponent (regression)

assert_checkerror('%s^%pi','Invalid exponent: expected finite integer exponents.')
assert_checkerror('%s^[1 %pi]','Invalid exponent: expected finite integer exponents.')
assert_checkerror('%s.^[1 %pi]','Invalid exponent: expected finite integer exponents.')
