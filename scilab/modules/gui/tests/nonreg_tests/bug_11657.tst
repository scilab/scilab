// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11657 -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11657
//
// <-- Short Description -->
// Setting the callback_type property of an uicontrol was not taken into account.

c = uicontrol(gcf(),"style","checkbox","string","Test","position", ..
[10,400,100,15],"callback","disp(""Hello"")");
assert_checkequal(c.callback_type, 0);

c.callback_type = -1;
assert_checkequal(c.callback_type, -1);

refMsg = msprintf(_("Wrong value for ''%s'' property: %d, %d, %d or %d expected.\n"), "callback_type", -1, 0, 1, 2);
assert_checkerror("c;c.callback_type = 100;", refMsg);

delete(gcf());
