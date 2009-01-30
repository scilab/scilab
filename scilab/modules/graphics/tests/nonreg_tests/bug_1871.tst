// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 1871 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1871
//
// <-- Short Description -->
// plot3d1: facet lines are erroneously drawn, if matrix datapoint is out of boundary box 
// 

xv = 1:10; yv=xv; s = rand(length(xv), length(yv));
min_z = min(s); max_z = max(s);

s(5, 5) = min_z-10;

//datapoint outside boundary box (same happens for %nan value)

plot3d1(xv,yv,s, flag=[-1 1 4], ebox = [min(xv), max(xv), min(yv), max(yv), min_z, max_z]);

a=gca(); a.view = "2d"; a.tight_limits = "on"; //make nice view

// check that no lines appear in the hole

a.rotation_angles = [60,60];

// check that the hole is clipped
