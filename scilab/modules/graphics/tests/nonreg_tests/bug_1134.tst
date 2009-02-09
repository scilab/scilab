// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1134 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1134
//
// <-- Short Description -->
// strf="120" in grayplot disables graphics title 
// strf= "122" doesn't create a box without tics 

grayTitle = ["My grayplot"; "with strf=120"];
x = 1:10;
m = rand(10,10);

axes = gca();
axes.title.text = grayTitle;
grayplot(x,x,m, '120');

// check that the title is still there
if (axes.title.text <> grayTitle) then pause; end

// do the second grayplot
clf();
axes = gca();
axes.title.text = grayTitle;
grayplot(x,x,m, '122');

// check also that tics are not drawn
if (axes.axes_visible <> ["off", "off", "off"]) then pause; end
if (axes.title.text <> grayTitle) then pause; end
