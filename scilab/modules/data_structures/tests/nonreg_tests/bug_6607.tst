// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 6607 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6607
//
// <-- Short Description -->
// Implicit creation of structure array with some syntaxes like
//  S(1:2, 1:3).a = 1; yielded an error

// All the following failed in Scilab 5.5.2. Should now work:
assert_checkequal(execstr("clear S; S(1:2,1:3).a = %pi", "errcatch"), 0);
assert_checkequal(execstr("clear S; S(1,1:2,1:3).a = %pi", "errcatch"), 0);
assert_checkequal(execstr("clear S; S(2,1:2,1:3).a = %pi", "errcatch"), 0);
assert_checkequal(execstr("clear S; S(1,1:2,1:3).a = %pi", "errcatch"), 0);
assert_checkequal(execstr("clear S; S(2,1:2,1:3).a = %pi", "errcatch"), 0);
assert_checkequal(execstr("clear S; S(1:2,1,1:3).a = %pi", "errcatch"), 0);
assert_checkequal(execstr("clear S; S(1:2,2,1:3).a = %pi", "errcatch"), 0);
assert_checkequal(execstr("clear S; S(1:2,1:2,1:3).a = %pi", "errcatch"), 0);
assert_checkequal(execstr("clear S; S(1:2,1:3,2).a = %pi", "errcatch"), 0);
