// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI Group - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14490 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14490
//
// <-- Short Description -->
// plot2d crashed when leg parameter was a vector. 

aa = [1 2 3]' * [0.2 0.3];
assert_checkfalse(execstr("plot2d(aa, leg = [''1''; ''2''])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A single string expected.\n"), "plot2d", 2);
assert_checkerror("plot2d(aa, leg = [''1''; ''2''])", refMsg);

assert_checkfalse(execstr("plot2d(aa, leg = [1; 2])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A single string expected.\n"), "plot2d", 2);
assert_checkerror("plot2d(aa, leg = [1; 2])", refMsg);