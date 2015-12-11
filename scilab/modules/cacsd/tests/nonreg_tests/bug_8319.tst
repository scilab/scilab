// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8319 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8319
//
// <-- Short Description -->
// dbphi(hypermat) and phasemag(hypermat) return a matrix instead of hypermat

x = rand(2,2,2);
c = x*(1+%i);
[dB phi] = dbphi(c);
assert_checktrue(length(size(phi)) > 2);
assert_checkequal(size(phi), size(x));
