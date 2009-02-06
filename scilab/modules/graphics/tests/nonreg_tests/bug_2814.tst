// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
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

expectedXTicks = [-200; -150; -100; -50; 0; 50; 100; 150; 200];
expectedYTicks = [-50; 0; 50; 100];

clf();
ax=gca();
ax.data_bounds=[-180,-50;180,90];
ax.box='on'; 
ax.axes_visible=["on","on"];
ax.tight_limits="on";
a=5*ones(51,50);
Matplot1(a,[-180,-50,180,90]);

// check X and Y ticks
if (ax.x_ticks.locations <> expectedXTicks) then pause; end
if (ax.y_ticks.locations <> expectedYTicks) then pause; end
if (ax.x_ticks.labels <> string(expectedXTicks) ) then pause; end
if (ax.y_ticks.labels <> string(expectedYTicks)) then pause; end
