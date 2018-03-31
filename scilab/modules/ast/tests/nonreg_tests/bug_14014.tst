// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14014 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14014
//
// <-- Short Description -->
// "end" could be directly followed by an inline instruction

assert_checktrue(execstr("if %t, a=1, end b=2", "errcatch")~=0);
assert_checktrue(execstr("a=%t; while a, a=%f, end b=2", "errcatch")~=0);
assert_checktrue(execstr("a=1; select a, case 1, b=3, end b=2", "errcatch")~=0);
