// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9158 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9158
//
// <-- Short Description -->
// zeros() called with a number greater than 2**31 returns an empty matrix

assert_checktrue(execstr("zeros(2**32, 1)", "errcatch") == 999);
assert_checktrue(execstr("zeros(1, 2**32)", "errcatch") == 999);

// the same with eye, rand, and ones
assert_checktrue(execstr("eye(2**32, 1)", "errcatch") == 999);
assert_checktrue(execstr("eye(1, 2**32)", "errcatch") == 999);

assert_checktrue(execstr("ones(2**32, 1)", "errcatch") == 999);
assert_checktrue(execstr("ones(2**32, 1)", "errcatch") == 999);

assert_checktrue(execstr("rand(2**32, 1)", "errcatch") == 999);
