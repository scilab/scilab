// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15886 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15886
//
// <-- Short Description -->
// display of polynomials is broken

assert_checkequal(string((1-%s)^3),["1 -3s +3s² -s³"]);
