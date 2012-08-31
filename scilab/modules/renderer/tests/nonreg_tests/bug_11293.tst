// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 11293 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11293
//
// <-- Short Description -->
// Line style was not taken into account

x=linspace(0,1,100);
plot2d(x,x);
e=gce();
p=e.children(1);
p.line_style=2;

// Check if a dashed line is drawn.