// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7364 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7364
//
// <-- Short Description -->
// The "Checked" property misworked for uimenus and they could not have a callback.

h=uimenu("label","Bug 7364")
h1=uimenu(h,"label", "Test for checked","checked","on")

function menu_toogle()
if get(gcbo, "checked") == "on" then
  disp("From checked to unchecked");
  set(gcbo, "checked", "off");
else
  disp("From unchecked to checked");
  set(gcbo, "checked", "on");
end
endfunction

h1.callback = "menu_toogle";

// Verify that sub-menu "Test for checked" is checked
// Click on sub-menu "Test for checked"
// "From checked to unchecked" mus be displayed
// Verify that sub-menu "Test for checked" is unchecked
// Click on sub-menu "Test for checked"
// "From unchecked to checked" mus be displayed

