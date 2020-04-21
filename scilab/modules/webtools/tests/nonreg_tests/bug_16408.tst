// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16408 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16408
//
// <-- Short Description -->
//    toJSON(var, filename, indent) is wrong and crashed.
//    toJSON(var, indent, filename) is the right call sequence. Documentation
//    has been udpated.
// =============================================================================

assert_checkfalse(execstr("toJSON([""a"" ""b""], tempname(), 1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: double expected.\n"), "toJSON", 2);
assert_checkerror("toJSON([""a"" ""b""], tempname(), 1)", refMsg);
