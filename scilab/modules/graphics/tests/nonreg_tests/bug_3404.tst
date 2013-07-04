// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3404 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3404
//
// <-- Short Description -->
// Some problems come when an axis is set in log scale.
//

// first check axes
a = gca();
a.data_bounds = [1e-9,-20;10,10];
a.axes_visible = "on";
a.log_flags = "lnn";

expectedLocations = 10^(-10:3:2)';

// compare the expected locations and the real ones
assert_checkalmostequal(expectedLocations, a.x_ticks.locations);

// now draw a segs object using this scale
xarrows([0.0001,0.01],[-15,1],5,1);

// check that the arrow is centered




