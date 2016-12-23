// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 13375 -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13375
//
// <-- Short Description -->
// If (Max - Min) == 1, then the multiple selection is not allowed.

f=figure();
h=uicontrol(f,"style","listbox", ...
"position", [10 10 150 160]);
set(h, "string", "item 1|item 2|item3");

assert_checkfalse(execstr("set(h, ''value'', [1 3])", "errcatch") == 0);

// Check we got the right error message
refMsg = msprintf(_("(Max - Min) must be greater than 1 to allow the multiple selection.\n"));
assert_checkerror("set(h, ''value'', [1 3])", refMsg);

close(f)
