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
// <-- Non-regression test for bug 16303 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16303
//
// <-- Short Description -->
// log10(x) has wrong dimensions when x is an hypermatrix

x = rand(2,3,4);
assert_checkequal(size(log10(x)),[2,3,4])