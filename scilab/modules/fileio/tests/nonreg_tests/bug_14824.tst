// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14824 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14824
//
// <-- Short Description -->
// Incorrect error message with mfprintf(fd, "%d", [])

fd = mopen('tmp.tmp', 'w');
str = "mfprintf(fd, ""%d"", [])";
assert_checkfalse(execstr(str   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input arguments: data doesn''t fit with format.\n"), "mfprintf");
assert_checkerror(str, refMsg);
mclose(fd)
