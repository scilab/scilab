// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2911 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2911
//
// <-- Short Description -->
// In scilab5 the grids can only be displayed if the axes_visible property is set to "on"
 

plot2d()
ax=gca();ax.axes_visible='off';ax.grid=[1 1 1];

// check that the grid is drawn even if the ticks are not


