// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7365 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7365
//
// <-- Short Description -->
// "delete" applied to a "Checked" uimenu destroyed the handle without updating the display.

m = uimenu(gcf(),"Label","Test")
m1 = uimenu(m, "Label","SubChecked","checked","off") 
// Look at the menu/submenu
delete(m1)
// Look at the menu/submenu: submenu must have been destroyed
is_handle_valid(m1)
// The handle must not be valid
