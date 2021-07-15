// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16476 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16476
//
// <-- Short Description -->
// issquare(1/%z) answers %F. Same issue for any other square matrix of rationals

g = 1/%z;
assert_checktrue(issquare(g));
assert_checkfalse(issquare([g g]));
assert_checktrue(issquare([g g;g g]));

