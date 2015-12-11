// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13494 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13494
//
// <-- Short Description -->
// Wrong vertical range of plot3d and plot3d1

// Check z axis is [20;30]
x = linspace(-1,1,4);y = linspace(-1,1,3);
Z = linspace(20,30,4)'*ones(y);
plot3d(x,y,Z)
