// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 2234 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2234
//
// <-- Short Description -->
// When using xs2gif or xs2bmp, when the graphic window is mimimized, the BMP files
// are not created, and the GIF files are flawed (size = 1 KB)
// 

plot2d;
// minimize window
fig = gcf();
xs2gif(fig.figure_id, TMPDIR+"/test.gif");
xs2bmp(fig.figure_id, TMPDIR+"/test.bmp");

// check the two generated files.

