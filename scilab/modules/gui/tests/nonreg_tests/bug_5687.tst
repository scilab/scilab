// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre LANDO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 5687 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5687
//
// <-- Short Description -->

// When setting "figure_name" the window title was not set until the window is "touched" with the mouse.

// Try this code and see if the title is changed instantly

f = gcf();
f.figure_name = 'Windows title test';
