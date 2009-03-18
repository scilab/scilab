// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4173 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4173
//
// <-- Short Description -->
// Graphic export : Style the text fonts is not respected when exporting to SVG.
// 

scf(10);
plot3d();
title('Title in bold-italic');

my_axe              = gca();
my_title            = my_axe.title;
my_title.font_size  = 5;
my_title.font_style = 5;

xs2svg(10,TMPDIR+'/bug_svg_bold_italic.svg');

// have a look to 'bug_svg_bold_italic.svg' and check that the title is bold and italic


