// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7082 -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7082
//
// <-- Short Description -->
// Setting the "enable" property had no effect on "listbox" and "text" uicontrols.


// Create a figure
h = gcf();
width  = 100;
height = 50;
step   = 60;

// Create a listbox
listbox = uicontrol("Parent", h, ...
    "Style", "listbox", ...
    "Position", [10 1*step width height], ...
    "String", "item 1|item 2|item3", ...
    "Value", [1 3], ...
    "Background", [1 1 1]);

// Create a label
label = uicontrol("Parent", h, ...
    "Style", "text", ...
    "Position", [10 2*step width height], ...
    "String", "label", ...
    "Background", [1 1 1]);

//
// Now, try to enable/disable the listbox and the label
//

set(listbox, "Enable", "off");
// Check that the listbox is not selectable and grayed.
set(listbox, "Enable", "on");
// Check that the listbox is selectable and has a white background.

set(label, "Enable", "off");
// Check that the label is not selectable and grayed.
set(label, "Enable", "on");
// Check that the label is selectable and is has a white background.
