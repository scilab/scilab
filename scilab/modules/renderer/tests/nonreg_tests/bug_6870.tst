// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre LANDO
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 6870 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6870
//
// <-- Short Description -->
// Arrow with infinite value was not plotted even if it was possible.


// Run this script and see the vertical arrow @ x=0

ieee(2);
x=linspace(-2,2,51);
y= exp(8*x) + 1 ./ abs(x);
plot(x,y);
e=gce();e=e.children;
e.polyline_style=4;
