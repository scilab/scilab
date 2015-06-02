// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13679 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13679
//
// <-- Short Description -->
// Invalid subticks in log scale after copy

// Check that there are 3 horizontal dashed lines between 2 graduations on y-axis
plot2d(linspace(1,1000,100), linspace(1d-18,1d-55,100));
a = gca();
a.axes_bounds=[0 0.37 1 0.3];
a.grid=[1 1];
a.grid_style = [3 3];
a.sub_ticks = [1 3];
a.log_flags="lln";