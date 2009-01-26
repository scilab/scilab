// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 1342 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1342
//
// <-- Short Description -->
// The representation of an Arc in 3d does not work .
// 

disp ("4.43.11B");
hf1 = scf (1);
hf1.figure_name = "4.43.11B1";
ha1 = hf1.children;
ha1.axes_visible = "on";
ha1.box = "on";
ha1.view = "3d";
ha1.grid = [2, 2];
xarc (0, 1, 1, 1, 0, 11520);
hc1 = gce ();
hc1.data (7) = 23040;
hc1.data (6) = 0;
hc1.data (5) = 1;
hc1.data (4) = 1;
hc1.data (3) = -1;
hf1.figure_name = "4.43.11B2";
tc1.data (3) = 1;


