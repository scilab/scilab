// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6786 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6786
//
// <-- Short Description -->
// Plotting polar graphs with regative rho values causes
// the polar coordinate system to be incorrectly drawn.
//

t = 0:0.01:2*%pi;

scf(0);
polarplot(t, -2 + sin(t));

// The coordinate system should fit the plotted curve, the polar
// grid's outer circle being tangent to the curve at rho=3 and t=pi/2,
// and rho ranging from 0 to 3 (instead of from 0 to -1).


scf(1);
polarplot(t, -1 + sin(t));

// The coordinate system should fit the plotted cardioid curve,
// instead of being compressed around the origin. The polar grid's
// outer circle should be tangent to the curve at rho=2 and t=pi/2,
// with rho ranging from 0 to 2.

