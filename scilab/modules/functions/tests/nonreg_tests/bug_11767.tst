// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 11767 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11767
//
// <-- Short Description -->
// execstr("A. 1","errcatch") failed to catch the error

assert_checkequal(execstr("execstr(""A. 1"", ""errcatch"")", "errcatch"), 0);
assert_checkequal(execstr("A. 1", "errcatch"), 999);
