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
// <-- Non-regression test for bug 16151 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16151
//
// <-- Short Description -->
// isequal(1:$, 2:$) returns %T

assert_checktrue(isequal(1:$,1:1:$));
assert_checktrue(isequal($-1:$,$-1:1:$));
assert_checkfalse(isequal($-1:$,$-1:2:$));
assert_checkfalse(isequal($:-1:1,1:$));
assert_checkfalse(isequal(1:$,2:$));
assert_checkfalse(isequal(1:$,1:2:$));
assert_checkfalse(isequal(1:$,1:$-1));
assert_checkfalse(isequal(1:$,1:2:$));


