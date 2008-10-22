// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Sylvestre Koumar
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3124 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3124
//
// <-- Short Description -->
// The arrow sizes are in scilab 5 given by absolute value.
// In Scilab 4 the size  where relative to the user coordinates.


clf();a=gca();
xsegs([0;0.5],[0;0.5]);e=gce();e.arrow_size=2;
a.data_bounds=[0 0;10 10];

// check that in Scilab 4 and 5, arrow size is the same.

