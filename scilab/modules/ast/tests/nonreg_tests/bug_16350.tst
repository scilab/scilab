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
// <-- Non-regression test for bug 16350 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16350
//
// <-- Short Description -->
// In if/while conditions, the empty sparse boolean is considered as TRUE.

a = sparse(1)<2;
a(1) = [];
b = 1;
if a, b = 2; end
assert_checkequal(b,1);
