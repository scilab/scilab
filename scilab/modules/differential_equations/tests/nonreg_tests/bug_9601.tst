// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
// <-- Non-regression test for bug 9601 -->
//
// <-- Bugzilla URL -->
//http://bugzilla.scilab.org/show_bug.cgi?id=9601
//
// <-- Short Description -->
// Cylinder demo fails with a bad value

cd SCI/modules/differential_equations/demos/flow
exec cylinder.dem.sce
// Put the slider "gravity" at the minimum and start the simulation
// Check that there is no error message in the console 
