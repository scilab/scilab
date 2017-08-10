// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2012 - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9934 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10278
//
// <-- Short Description -->
// clear(["A","B"]) returned a non standard error message

// No error issued since bug #8373 fix.

ierr = execstr("clear([""A"",""B""])", "errcatch");
assert_checkequal(ierr, 0);
ierr = execstr("clear([""A"",""B""])", "errcatch");
assert_checktrue(isempty(lasterror()));
