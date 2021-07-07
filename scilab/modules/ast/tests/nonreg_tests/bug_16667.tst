// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16667 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16667
//
// <-- Short Description -->
// ./ is broken for a RHS having 0 (recent regression. 6.1.0 was OK)

assert_checkequal(1./[1 0],[1 %inf])
assert_checkequal(1./[1 -0],[1 -%inf])
