// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13588 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13588
//
// <-- Short Description -->
// default properties (git comm) of figures badly inherited.

function handler(w,x,y,k)
    disp(k)
endfunction

// When set on dockable figure
set(gdf(),"event_handler","handler","event_handler_enable","on");
gcf();
// Move the mouse over the figure and check that messages are displayed in Scilab console
// Close the figure and check that -1000 has been displayed in Scilab console

// When set on non-dockable figure
set(gdf(),"event_handler","handler","event_handler_enable","on");
figure("Dockable", "off");
// Move the mouse over the figure and check that messages are displayed in Scilab console
// Close the figure and check that -1000 has been displayed in Scilab console
