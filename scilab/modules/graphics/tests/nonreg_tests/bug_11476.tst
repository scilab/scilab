// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 11476 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11476
//
// <-- Short Description -->
// clf('reset') applied to a docked figure moved and resized the whole block
//              of docked objects, likely the Scilab's desktop.

f = scf();
plot2d()
// Please dock the figure to the desktop
fp = f.figure_position;
fs = f.figure_size;
as = f.axes_size;

clf('reset')
// The Scilab's desktop shall not have been moved nor resized
assert_checkequal(f.figure_position, fp);
assert_checkequal(f.figure_size, fs);
assert_checkequal(f.axes_size, as);
