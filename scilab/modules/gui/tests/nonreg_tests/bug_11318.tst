// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11318 -->
// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11318
//
// <-- Short Description -->
// Order of uimenus in a figure changed when adding submenus.

// 1 - Create a figure with uimenus
f = scf();
x0 = uimenu("parent", f, "label", "menu_0");
x1 = uimenu("parent", x0, "label", "menu_1");
x2 = uimenu("parent", x0, "label", "menu_2");
x3 = uimenu("parent", x0, "label", "menu_3");

// Check that children in menu_0 are: menu_1, menu_2, menu_3 (in that order).

// 2 - Add one submenu to menu_1
x11 = uimenu("parent", x1, "label", "menu_11");

// Check that children in menu_0 are: menu_1, menu_2, menu_3 (in that order).
// Check that menu_1 has a child named menu_11
