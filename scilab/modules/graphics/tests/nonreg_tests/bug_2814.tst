// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2814-->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2814
//
// <-- Short Description -->
//
// Ticks graduation produce some unwanted decimals.
//

drawlater;

clf();
ax=gca();
ax.data_bounds=[-180,-50;180,90];
ax.box='on'; 
ax.axes_visible=["on","on"];
ax.tight_limits="on";
a=5*ones(51,50);
Matplot1(a,[-180,-50,180,90]);

drawnow;
sleep(500); //Wait for MVC update
// check X ticks locations are integers
for kTick=1:size(ax.x_ticks.locations, "*")
    assert_checkequal(int(ax.x_ticks.locations(kTick)), ax.x_ticks.locations(kTick));
end
// check Y ticks positions are integers
for kTick=1:size(ax.y_ticks.locations, "*")
    assert_checkequal(int(ax.y_ticks.locations(kTick)), ax.y_ticks.locations(kTick));
end

// check X ticks labels match locations
assert_checkequal(ax.x_ticks.labels, string(ax.x_ticks.locations));
// check Y ticks labels match locations
assert_checkequal(ax.y_ticks.labels, string(ax.y_ticks.locations));
