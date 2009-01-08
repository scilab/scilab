// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 3910 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3910
//
// <-- Short Description -->
// In Scilab 4 and in isoview mode, some lines don't superpose although they should.
// 

frame_size = 4.e-6
a=get("current_axes");
a.data_bounds = [0, 0; frame_size, frame_size ];
a.isoview = "on";
a.axes_visible = "on";
//line 1
xpoly([0, 1.e-4], [0, 1.e-4])
//line 2
xpoly([0, 1.e-3], [0, 1.e-3])

// only one line should be seen since the two superpose
