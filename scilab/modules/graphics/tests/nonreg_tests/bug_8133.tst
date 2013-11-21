// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 8133 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8133
//
// <-- Short Description -->
// Ticks disappeared in planar 3D view

clf();
plot3d([1,2],[1,2],[1,2]);

// ticks should be present on z-axis & x-axis
set(gca(),"rotation_angles",[90,90]);

// ticks should be present on z-axis & y-axis
set(gca(),"rotation_angles",[90,0]);