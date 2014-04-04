// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3158 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3158
//
// <-- Short Description -->
// In a 2D plot, when the axes of the figure are in "origin" mode,
// the zoom works very bad because the (0,0) point remains in the figure.

a=gca(); a.x_location="origin"; a.y_location="origin";
x=linspace(0,1,100); plot(x,x)

// and now make a zoom on a part of the plot not including the (0,0) point
zoom_rect([0.5,0.5,0.6,0.6]);

// check that 0 is not included in the frame
a.margins = [0,0,0,0];

// get user coordinates of the upper left point of the window
[x, y] = xchange(0, 0, "i2f");

if ( abs(x - 0.5) > 0.02 | abs(y - 0.6) > 0.02) then pause;end



