// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6835 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6835
//
// <-- Short Description -->
// Performing interactive rotations after having executed a plot3d command
// causes Scilab (64-bit version) to freeze on Windows 64-bit
//

plot3d();

// Perform several successive interactive rotations, moving the
// mouse as randomly as possible. Check whether freezes occur
// or not.

