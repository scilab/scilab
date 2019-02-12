// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15490 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15940
//
// <-- Short Description -->
// Error message of grand function was incoherent

assert_checkfalse(execstr("grand(2,2,2,""uin"",0,2^31)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input arguments #%d and #%d: Low and High must be a 32 bits integer value and (high - low + 1) <=  2147483561.\n"), "grand", 5, 6);
assert_checkerror("grand(2,2,2,""uin"",0,2^31)", refMsg);
