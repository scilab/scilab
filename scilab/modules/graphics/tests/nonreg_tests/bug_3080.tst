// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 3080 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3080
//
// <-- Short Description -->
// When we click many times on a button of the Graphic window the instruction will be done as much times we have clicked.
// 

plot3d();
rotate_axes();
rotate_axes();
rotate_axes();
zoom_rect();
zoom_rect();
unzoom();
rotate_axes();
zoom_rect();
rotate_axes();
unzoom();
unzoom();
zoom_rect();
zoom_rect();
rotate_axes();

// only the last rotae_axes should be active
