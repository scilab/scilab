// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7368 -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7368
//
// <-- Short Description -->
// Setting the "Visible" property to "off" had no effect on "Checked" uimenus.

mainMenu = uimenu(gcf(),"Label","Menu");
subMenu = uimenu(mainMenu, "Label", "Menu Item");
// Check that the Menu Item has been created and is visible
subMenu.visible = "off";
// Check that the Menu Item is no more visible
subMenu.visible = "on";
// Check that the Menu Item is visible
subMenu.checked = "on";
// Check that the Menu Item is visible and checked
subMenu.visible = "off";
// Check that the Menu Item is no more visible
subMenu.visible = "on";
// Check that the Menu Item is visible