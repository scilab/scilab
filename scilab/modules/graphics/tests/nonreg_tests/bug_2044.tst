// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2044 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2044
//
// <-- Short Description -->
// Bug with XOR mode, nothing drawn till the window is resized or exposed

f=scf(0);clf()
//An garphic figure without margins, units from 0 to 100.
ax=gca();
ax.data_bounds=[0 0 ;10 10];f.pixel_drawing_mode = "xor";
xrects([2;2;1;1],8)
xrects([3;3;1;1],8)

// Check that there is is 2 black rectangles on screen

