// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16228 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16228
//
// <-- Short Description -->
// --> clf, bode, nyquist yielded an error
// bode() and nyquist() inline demos did not clear and reset the current axes
// area where to draw

// Run all the following lines. This must yield no error
clf
subplot(1,2,1)
plot2d
bode

// EXPECTED DISPLAY:
// The plot2d example must have been cleared.
// The bode() example must be displayed on the left half of the figure

subplot(1,2,2)
plot2d
nyquist
// EXPECTED DISPLAY:
// The plot2d example must have been cleared on the right.
// The nyquist() example must be displayed instead.
// The bode example is still displayed on the left side.
