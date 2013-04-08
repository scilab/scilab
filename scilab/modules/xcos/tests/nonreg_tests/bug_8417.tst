// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8417 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8417
//
// <-- Short Description -->
// A window appears when openning an h5 diagram file.

xcos();
pause,
// check that a window is open
closeXcos();

xcos("modules/xcos/demos/Bouncing_ball.zcos");
pause,
// check that a window is open with a complete diagram on it
closeXcos();


// creating a huge diagram
x = rand(400, 1) * 800;
y = rand(400, 1) * 600;
scs_m = scicos_diagram();
for i=1:100
	o = BIGSOM_f("define");
	o.graphics.orig = [x(i) y(i)];
	o.graphics.sz = o.graphics.sz * 10;
	scs_m.objs(0) = o;
end

xcos(scs_m)
pause,
// check that a window is open with a complete diagram on it
closeXcos();

