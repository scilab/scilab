// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6535 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6535
//
// <-- Short Description -->
// The property ListboxTop of the graphic user interface object listbox have no effect.

// Create the test figure
x=string([1:20]');
h=figure();
g=uicontrol(h, ...
	"Style", "listbox", ...
	"String", x, ...
	"Position", [10,10,100,100], ...
	"Fontsize", 15, ...
	"BackgroundColor", [1,0,0]);

// Check that item with label '1' is the top item in the listbox

set(g, "ListboxTop", 4);

// Check that item with label '4' is the top item in the listbox

get(g, "Listboxtop")

// Check that the value returned is 4