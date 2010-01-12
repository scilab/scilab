// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-20010 - DIGITEO - Pierre LANDO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 6420 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6420
//
// <-- Short Description -->
// It was not possible to use ax.{x, y, z}_ticks with LaTex commands.

x = -2 * %pi : %pi / 100 : 4 * %pi;
y =sin(x);
fig = figure(1);
fig.figure_size = [700,700];
fig.axes_size = [700,700];
fig.background =  -2;
fig.figure_name = "Mathe Plot";

ax = gca();
ax.grid = [1,1];
ax.auto_ticks = ["on","on","on"];

ax.x_ticks = tlist(['ticks','locations','labels'],[-2;-1.5;-1;-0.5;0;0.5;1;1.5;2;2.5;3;3.5;4]*%pi,["$\scalebox{0.6}{-2\pi}$";"$\scalebox{0.6}{\frac{-3\pi}{2}}$";"$\scalebox{0.6}{-\pi}$";"$\scalebox{0.6}{\frac{-\pi}{2}}$";"";"$\scalebox{0.6}{\frac{\pi}{2}}$";"$\scalebox{0.6}{\pi}$";"$\scalebox{0.6}{\frac{3\pi}{2}}$";"$\scalebox{0.6}{2\pi}$";"$\scalebox{0.6}{\frac{5\pi}{2}}$";"$\scalebox{0.6}{3\pi}$";"$\scalebox{0.6}{\frac{7\pi}{2}}$";"$\scalebox{0.6}{4\pi}$"]);
plot2d(x,y);

