// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 3634 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3634
//
// <-- Short Description -->
// Plot3d does not paint the surfacess defined by facets with 5 vertices.
// 

scf(0);clf();
plot3d([1;1;1;1;1],[1;2;3;4;5],[0;3;2;3;1])

// check that the inside is displayed
// could use image processing for that
