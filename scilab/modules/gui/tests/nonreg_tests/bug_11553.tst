// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11553 -->
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11553
//
// <-- Short Description -->
// The uicontextmenu handles (and their children) were not destroyed at the end of execution.

// 1 - Copy/paste the following lines into Scilab
// Create a context menu
hMenu = uicontextmenu();
// Create sub menus
hItem1 = uimenu("Label", "Item1", "Parent", hMenu, "Callback", "disp(""Item1 clicked!"")");
hItem2 = uimenu("Label", "Item2", "Parent", hMenu, "Callback", "disp(""Item2 clicked!"")");
hItem3 = uimenu("Label", "Item3", "Parent", hMenu, "Callback", "disp(""Item3 clicked!"")");

// 2 - Click on an item

// 3 - Copy/paste the following lines into Scilab (all must be OK)
assert_checkfalse(is_handle_valid(hMenu));
assert_checkfalse(is_handle_valid(hItem1));
assert_checkfalse(is_handle_valid(hItem2));
assert_checkfalse(is_handle_valid(hItem3));
