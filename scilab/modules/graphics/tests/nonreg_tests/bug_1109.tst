// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1109 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1109
//
// <-- Short Description -->
// Argument axesflag=2 does not work with plot2d
//

 
// check all possible combinations
x = 1:10;

// axesflag=0 nothing around the plot
clf();
plot2d(x, x, axesflag=0);
axes = gca();
if (axes.axes_visible <> ["off","off","off"]) then pause; end
if (axes.box <> "off") then pause; end;

// axesflag=1: axes + box + y left
clf();
plot2d(x, x, axesflag=1);
axes = gca();
if (axes.axes_visible <> ["on","on","on"]) then pause; end
if (axes.box <> "on") then pause; end;
if (axes.y_location <> "left") then pause; end

// axesflag=2: box + no axes
clf();
plot2d(x, x, axesflag=2);
axes = gca();
if (axes.axes_visible <> ["off","off","off"]) then pause; end
if (axes.box <> "on") then pause; end;

// axesflag=3: axes + !box + y right
clf();
plot2d(x, x, axesflag=3);
axes = gca();
if (axes.axes_visible <> ["on","on","on"]) then pause; end
if (axes.box <> "off") then pause; end;
if (axes.y_location <> "right") then pause; end

// axesflag=4: axes + x centered + y centered
clf();
plot2d(x, x, axesflag=4);
axes = gca();
if (axes.axes_visible <> ["on","on","on"]) then pause; end
if (axes.x_location <> "middle") then pause; end
if (axes.y_location <> "middle") then pause; end

// axesflag=5: axes + box + x centered + y centered
clf();
plot2d(x, x, axesflag=5);
axes = gca();
if (axes.axes_visible <> ["on","on","on"]) then pause; end
if (axes.box <> "on") then pause; end;
if (axes.x_location <> "middle") then pause; end
if (axes.y_location <> "middle") then pause; end

// axesflag=9: axes + !box + y left
clf();
plot2d(x, x, axesflag=9);
axes = gca();
if (axes.axes_visible <> ["on","on","on"]) then pause; end
if (axes.box <> "off") then pause; end;
if (axes.y_location <> "left") then pause; end

