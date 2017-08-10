// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 14517 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14517
//
// <-- Short Description -->
//    The second argument of part function must not contain of 0.
// =============================================================================
assert_checkfalse(execstr("part(""abcd"", [0 2])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong values for input argument #%d: Must be >= 1.\n"), "part", 2);
assert_checkerror("part(""abcd"", [0 2])", refMsg);
