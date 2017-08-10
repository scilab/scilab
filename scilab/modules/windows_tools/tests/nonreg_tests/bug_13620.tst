// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 13620 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13620
//
// <-- Short Description -->
//    dos function called with a vector as input crashed.
// =============================================================================

assert_checkfalse(execstr("dos([""dir"", ""dir""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"), "dos", 1);
assert_checkerror("dos([""dir"", ""dir""])", refMsg);

assert_checkfalse(execstr("dos([""dir""; ""dir""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"), "dos", 1);
assert_checkerror("dos([""dir"", ""dir""])", refMsg);
