// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 7842 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7842
//
// <-- Short Description -->
// There was not the filepath in the title bar.

scinotes("SCI/modules/graphics/demos/2d_3d_plots/plotyyy.dem.sce");

// the window's title must be the full path of plotyyy.dem.sce
