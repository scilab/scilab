// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 8842 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8842
//
// <-- Short Description -->
// Assigning the pointer of a built-in function to a new structure failed

assert_checkequal(execstr("s.f = cos;", "errcatch"),0);
clear s
assert_checkequal(execstr("s(2).f = cos;", "errcatch"),0);
clear s
assert_checkequal(execstr("s(2,3).f = cos;", "errcatch"),0);
