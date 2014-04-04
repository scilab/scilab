// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 6191 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6191
//
// <-- Short Description -->
// It was not possible to set thickness and line_style properties for a grid

clf();
x = [0:0.05*%pi:2*%pi]';
plot2d(x, sin(x))
xgrid(5, 1, 7);
a = gca();
a.grid_thickness = [2 2];
a.grid_style = [1 5];
