// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13592 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13592
//
// <-- Short Description -->
// When interactively setting a legend() in an axes embedded in a uicontrol frame,
// a varying shift could exist between the mouse position and the block of legends

fig = scf();
drawlater
fig.axes_size = [990,670];
fig.layout = "border";
f = uicontrol(fig, "style", "frame", ...
    "layout", "gridbag");

f1 = uicontrol(f, "style", "frame", ...
    "layout", "gridbag", ...
    "margins", [2,10,4,10], ...
    "constraints", createConstraints("gridbag", [1 1 1 1], [0.5 1], "both", "left"));

f2 = uicontrol(f, "style", "frame", ...
    "layout", "gridbag", ...
    "constraints", createConstraints("gridbag", [2 1 1 1], [0.5 1], "both", "left"));

a21 = newaxes(f2);
t = linspace(0,%pi,20);
plot2d("ll",t+0.1,1.01+[cos(t'),cos(2*t'),cos(3*t')],[-5,2 3]);
drawnow
legend(['cos(t)';'cos(2*t)';'cos(3*t)'], 5);
// Here: the upper left corner of the block of legends must follow accurately
// the mouse pointer over the whole frame area.
