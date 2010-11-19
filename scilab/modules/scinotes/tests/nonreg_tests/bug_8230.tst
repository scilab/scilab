// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8230 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8230
//
// <-- Short Description -->
// Save As action did not propose the current file name as default.

edit SCI/modules/graphics/demos/2d_3d_plots/champ.dem.sce

// ctrl+shift+S to save as and champ.dem.sce should be proposed rather
// than demo_champ.sci
