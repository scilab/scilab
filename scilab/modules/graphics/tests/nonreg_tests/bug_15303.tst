// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15303 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15303
//
// <-- Short Description -->
// Some properties written in capital letter in set function crashed Scilab.

f = figure();
ui = uicontrol(f);

assert_checkfalse(execstr("set(ui, ""text"", ""a"")"   ,"errcatch") == 0);
refMsg = msprintf(_("This object has no %s property.\n"), "text");
assert_checkerror("set(ui, ""text"", ""a"")", refMsg);

assert_checkfalse(execstr("set(ui, ""Text"", ""a"")"   ,"errcatch") == 0);
refMsg = msprintf(_("This object has no %s property.\n"), "text");
assert_checkerror("set(ui, ""Text"", ""a"")", refMsg);
