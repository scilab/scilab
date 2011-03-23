// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 5284 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5284
//
// <-- Short Description -->
// In some case function "legend" drew line segments in its box in a reverse order.
//
// The 3 figures created by this code should show two curve and two legends.
// The first legend should be "sin x", the second "cos x"
// The legends should correspond to the curves. ie: cos(0) = 1, sin(0) = 0.

x = linspace(0, 2*%pi);

scf(0);
plot(x,sin(x), x,cos(x));
legend("sin x", "cos x");

scf(1);
plot(x,sin(x), x,cos(x));
legend(["sin x", "cos x"]);

scf(2);
plot2d(x, [sin(x) ; cos(x)]', leg="sin x@cos x");
