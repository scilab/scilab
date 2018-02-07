// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13984 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13984
//
// <-- Short Description -->
// plot(.., "color", colors)
//  - did not accept a vector of colors for multiple curves
//  - did accept only 10 named colors instead of the full list of predefined ones

x = (-5:0.2:5)';
clf
subplot(1,3,1)
plot(x,[sin(x) cos(x)])   // Default colors

subplot(1,3,2)
plot(x,[sin(x) cos(x)], "color", ["orange" "#99BB22"])

subplot(1,3,3)
plot(x,[sin(x) cos(x)], "color", [ 255 0 0; 0 200 0]/255)

