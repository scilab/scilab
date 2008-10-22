// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// non regression bug for xclick in drawlater mode

// draw something
plot2d;

// first xclick to test
xclick()

drawlater();

// no redraw performed yet
xclick()

// for a reshape and redraw
f = gcf();
f.figure_size = 2 * f.figure_size;

// check now, before bug fix it returned [nan, nan]
xclick()


