// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 3660 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3660
//
// <-- Short Description -->
// Graphic export : font_angle was lost on the axis label when exporting through xs2pdf 
// 
plot3d();
xtitle("My title","my x axis label", "Volume","the Z axis");
a=gca();
a.y_label.font_angle=50;
a.y_label.font_size=8;
xs2pdf(0,TMPDIR+"/plop.pdf");
disp(TMPDIR+"/plop.pdf");
// have a look to 'TMPDIR+"/plop.pdf"' and check that the y label has been rotated


