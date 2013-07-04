// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11686 -->
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11686
//
// <-- Short Description -->
// An error occurred when clicking outside of an uicontextmenu area.

h0 = uicontextmenu();
h1 = uimenu("Label", "Item1", "Parent", h0);
h2 = uimenu("Label", "Item2", "Parent", h0);

b = uicontrol("String", "Click me");

// Click on the button:
// - the contextual menu must disappear
// - check that you have no Java exception