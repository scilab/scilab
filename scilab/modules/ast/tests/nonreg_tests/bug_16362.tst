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
// <-- Non-regression test for bug 16362 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16362
//
// <-- Short Description -->
// sparse([]) can no longer be concatenated

esp = sparse([]);
a = [1 2;3 4];
asp = sparse(a);

assert_checkequal([a, esp], asp);
assert_checkequal([esp, a], asp);
assert_checkequal([a; esp], asp);
assert_checkequal([esp; a], asp);

assert_checkequal([asp, esp], asp);
assert_checkequal([esp, asp], asp);
assert_checkequal([asp; esp], asp);
assert_checkequal([esp; asp], asp);