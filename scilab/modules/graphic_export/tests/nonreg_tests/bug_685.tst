// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Jean-Baptiste Silvy <jean-baptiste.silvy@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 685 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=685
//
// <-- Short Description -->
// Export to GIF ignores xlfont settings
// 

if getos() == 'Windows' then
	my_font = "Arial";
else
	my_font = "Monospaced";
end

if find( xlfont('AVAILABLE_FONTS') == my_font ) <> [] then
	xlfont(my_font,5);
	plot2d()
	a = gca();
	a.title.font_style = 5;
	a.title.font_size = 3;
	xtitle('this should be Arial!');
end

xs2gif(0, TMPDIR + "/test_bug_685.gif");

// check that the font is the same on the screen and the exported file.
