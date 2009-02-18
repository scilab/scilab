// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 3873 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3873
//
// <-- Short Description -->
// While editing ticks'label on Axes, mouse click outside box or double clicks or using Tab key, program terminates.
// 

plot2d;
fig = gcf();
ged(9, fig.figure_id);

// open x ticks window and move quickly between labels position and text using the tab key.
// Do this several times, the bug used to open only very seldom.
