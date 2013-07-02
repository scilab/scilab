// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2012 - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9934 -->
//
// <-- JVM NOT MANDATORY -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10278
//
// <-- Short Description -->
// clear(["A","B"]) returned a non standard error message

ierr = execstr("clear([""A"",""B""])", "errcatch");
assert_checkequal(ierr, 999);
msg_ref = msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"), "clear", 1);
assert_checkerror ("clear([""A"",""B""])", msg_ref);
