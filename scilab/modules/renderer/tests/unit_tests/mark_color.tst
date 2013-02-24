// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Unit-test for color in marks -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/
//
// <-- Short Description -->
// Tests that the marks can be coloured in 2D plots with offset and stride set
// 
// <-- INTERACTIVE TEST -->

x = (1:100);
rand("normal");
y_1 = 5 * (x + 15*rand(1,100)) + 10;
y_2 = 15 * (x + 10 *rand(1,100)) - 18;

clf();
drawlater();
plot(x, y_1, ".c");
plot(x, y_2, ".c");

fig = gcf();
fig.color_map = rainbowcolormap(32);
ax = gca();
pl_2 = ax.children(1).children(1);
pl_2.mark_mode = "on";
pl_2.mark_offset = 3;
pl_2.mark_background = -3;
pl_2.mark_stride = 5;
pl_2.mark_size_unit = "point";
pl_2.mark_size = 10;
pl_2.colors = grand(1, 100, "uin", 1, 32);


pl_1 = ax.children(2).children(1);
pl_1.mark_mode = "on";
pl_1.mark_offset = 3;
pl_1.mark_background = -3;
pl_1.mark_stride = 5;
pl_1.mark_size_unit = "point";
pl_1.mark_size = 20;
pl_1.colors = grand(1, 100, "uin", 1, 32);
drawnow();
rand("uniform");
 
