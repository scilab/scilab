// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 7492 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7492
//
// <-- Short Description -->
// The Grayplot object's data is transposed when displayed if its
// data_mapping property is set to direct.
//

// Two-level matrix to plot
z = ones(7,7);
z(2:3,3:4) = 3;
x = 1:7;
y = 1:7;

// Grayplot with direct data mapping
f = scf();
grayplot(x,y,z);
f.color_map = jetcolormap(3);
f.children(1).children(1).data_mapping = "direct";
f.children.title.text = "Direct Mapping";

// The rectangle should exactly fit to the boundary of
// the red-colored part of the grayplot object.
xrect(2,5,2,2);
set(gce(),"thickness",3);

