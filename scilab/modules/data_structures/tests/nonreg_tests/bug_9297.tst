// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 9297 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9297
//
// <-- Short Description -->
// assigning a mlist() to a structure's field failed

A = struct();
B = mlist(['V','toto'],5);
assert_checkequal(execstr("A.titi = B;", "errcatch"),0);
assert_checkequal(typeof(A.titi), "V");
clear A
assert_checkequal(execstr("A.titi = B;", "errcatch"),0);
assert_checkequal(typeof(A.titi), "V");
clear A
assert_checkequal(execstr("A(2,3).titi = B;", "errcatch"),0);
