// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 3956 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3956
//
// <-- Short Description -->
// Changing legend font style is taken into account.
// 

x = 0:0.1:2 * %pi;
plot2d(x,sin(x),leg="sin(x)");
axes = gca();
leg = axes.children(2);
leg.font_style = 1;
leg.font_style = 2;

// legend font style should be modified
