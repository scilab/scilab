// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 6608 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6608
//
// <-- Short Description -->
// Field and data insertions in an array of structures might fail


S(1:6).a = "abc";
assert_checkequal(execstr("S(1:6).b = %pi", "errcatch"), 0);
assert_checkequal(execstr("S(:).b = %pi", "errcatch"), 0);
assert_checkequal(execstr("S.b = S", "errcatch"), 0);
assert_checkequal(size(S),[6 1]);

clear S
S(2,2).a = "abc";
assert_checkequal(execstr("s.b = ''text''", "errcatch"), 0);
assert_checkequal(execstr("s(:).b = ''text''", "errcatch"), 0);
assert_checkequal(execstr("s(:).c = ''text''", "errcatch"), 0);
