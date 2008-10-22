// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 217 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=217
//
// <-- Short Description -->
// Successive use of 2d zoom in a graphics window gives obscure results.

t=0:0.1:2000;
plot(t,sin(t));

// zooming on one period used to give bad result.
x = 1410;
axes = gca();
axes.zoom_box = [x, -1, x + 2 * %pi, 1];

// check the display

