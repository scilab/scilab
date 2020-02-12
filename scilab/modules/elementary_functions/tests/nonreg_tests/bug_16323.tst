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
// <-- Non-regression test for bug 16323 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16323
//
// <-- Short Description -->
// conj(sparse(x)) is complex even when x is real

x = [1,2;3 4]
assert_checktrue(isreal(conj(sparse(x))))
