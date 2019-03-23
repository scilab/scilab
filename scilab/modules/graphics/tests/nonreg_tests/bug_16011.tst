// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16011 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16011
//
// <-- Short Description -->
// colorbar(umin, umax) failed after plot3d2()

u = linspace(-%pi/2,%pi/2,40);
v = linspace(0,2*%pi,20);
X = cos(u)'*cos(v);
Y = cos(u)'*sin(v);
Z = sin(u)'*ones(v);
plot3d2(X,Y,Z)
assert_checktrue(execstr("colorbar(min(Z), max(Z))", "errcatch")==0);
