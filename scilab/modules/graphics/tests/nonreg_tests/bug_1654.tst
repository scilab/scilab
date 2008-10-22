// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1654 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1654
//
// <-- Short Description -->
// Different behaviour of the graphic window under Linux and windows

hfd = gdf ();
hfd. figure_size = [300, 300];
had = gda ();
had.axes_visible = "on";
had.box = "on";
hf1 = scf (1);
hf1.figure_position = [0, 0];
hf2 = scf (2);
hf2.figure_position = [350,0];

// check that the figures don't overlap
      

