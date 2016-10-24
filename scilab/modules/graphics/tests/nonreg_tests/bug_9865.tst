// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 9865 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9865
//
// <-- Short Description -->
// When making a plot with "point" (no line), no symbol was shown in the legend.

x=0:0.1:6;
y=sin(x);
px = [ 0.2 1.0 4.0  5.0];
py = [ 0.2 0.7 0.7 -0.4];

plot(x,y,"b-");
plot(px,py,"r.");

legend( "function", "points" );

// check that the point is shown in the legend
