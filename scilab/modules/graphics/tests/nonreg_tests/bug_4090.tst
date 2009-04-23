// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4090 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4090
//
// <-- Short Description -->
// draw does not work any more while inside drawlater/drawnow scope.
// 

x = 1:10;

subplot(212)
a=gca();
plot2d(1:10);
drawlater

subplot(211);
plot2d(x, sin(x)) // default drawing mode

e=gce();
draw(e.children(1));

// should draw a polyline inside the top axes

