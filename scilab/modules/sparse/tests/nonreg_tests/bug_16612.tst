// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16612 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16612
//
// <-- Short Description -->
// kron() did not map .*. for sparse numeric argins

m = 0:2;
sp = sparse(m);
assert_checkequal(kron(sp, m), sp .*. m);
assert_checkequal(kron(sp, sp), sp .*. sp);
assert_checkequal(kron(m, sp), m .*. sp);
