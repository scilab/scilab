// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1479 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1479
//
// <-- Short Description -->
// Difference in line style between windows and Linux

xrect (0, 1, 1, 1);
hr1 = gce ();
hr1.line_style = 1;
xrect (0, 1, 0.9, 0.9);
hr2 = gce ();
hr2.line_style = 1;
xrect (0, 1, 0.8, 0.8);
hr3 = gce ();
hr3.line_style = 2;
xrect (0, 1, 0.7, 0.7);
hr4 = gce ();
hr4.line_style = 3;
xrect (0, 1, 0.6, 0.6);
hr5 = gce ();
hr5.line_style = 4;
xrect (0, 1, 0.5, 0.5);
hr6 = gce ();
hr6.line_style = 5;
xrect (0, 1, 0.4, 0.4);
hr7 = gce ();
hr7.line_style = 6;
xrect (0, 1, 0.3, 0.3);
hr8 = gce ();
hr8.line_style = 7;
xrect (0, 1, 0.2, 0.2);
hr9 = gce ();
hr9.line_style = 8;
xrect (0, 1, 0.1, 0.1);
hr10 = gce ();
hr10.line_style = 9; 

// check that the results are the same between windows and Linux.


               





