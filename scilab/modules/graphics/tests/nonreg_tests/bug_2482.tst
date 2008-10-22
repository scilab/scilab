// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2482 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2482
//
// <-- Short Description -->
// Assigning a polyline_style of 6 or 7 can cause Scilab 4.1.1 to crash.

param3d(1:16, 1:16, 1:16);
param3d(2+(1:16), 3 * (1:16), 4 + (1:16));
h = gce();
// used to crash Scilab
h.polyline_style = 6;
h.polyline_style = 7;




