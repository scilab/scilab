// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aime AGNEL
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 14374 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14374
//
// <-- Short Description -->
// Shellmode did not parse comments properly

// This script is executed in SCI/modules/ast/unit_tests/parser.tst
// This is done to centralize parsing specific problems
// and because shell mode needs its specific testing script

ierr = execstr("convstr a u // a comment", "errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("convstr a /* a comment */", "errcatch");
assert_checkequal(ierr, 0);

ierr = execstr(["convstr a u /* some text"; ..
                "              ending comment here */"], "errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("convstr a /* some comments */ u;", "errcatch");
assert_checkequal(ierr, 0);
