// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15590 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15590
//
// <-- Short Description -->
// pdiv(), and then modulo() and pmodulo() failed with hypermatrices
// of polynomials

ph = ones(2,2,2)*%s;
assert_checkequal(pdiv(ph, ph), ones(2,2,2));
r = rand(2,2,2)-0.5;
assert_checkequal(modulo(ph+r, ph), r);
assert_checkequal(pmodulo(ph+r, ph), r);
