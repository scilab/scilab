// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3094 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3094
//
// <-- Short Description -->
// There is a white background behind each charater of tics.

plot3d();

axes = gca();
axes.font_size = 5;
axes.rotation_angles = [90,100];

// check that ticks labels are transparent

