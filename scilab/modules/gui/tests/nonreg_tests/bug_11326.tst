// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11326 -->
//
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11326
//
// <-- Short Description -->
// set(h, "enable", "off") and set(h, "visible", "off") after menu creation had no effect

// Create a figure with menus
f = scf();
x0 = uimenu("parent", f, "label", "menu_0");
x1 = uimenu("parent", x0, "label", "menu_1", "callback", "disp(1)");
x2 = uimenu("parent", x0, "label", "menu_2", "callback", "disp(2)");

set(x1, "enable", "off");
// Check that menu_1 is disabled, gray and its callback can not be executed

set(x2, "visible", "off");
// Check that menu_2 is invisible
