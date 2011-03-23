// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 7187 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7187
//
// <-- Short Description -->
// Plotting, zooming into a certain area of the plot and
// then plotting again causes Scilab to hang.
//

plot2d([0,10], [-1,1]);

zoom_rect([0,-1,0.5,0]);

plot2d([-1,-1], [0,0]);
// The above plot2d command used to freeze Scilab,
// which should not occur anymore.

