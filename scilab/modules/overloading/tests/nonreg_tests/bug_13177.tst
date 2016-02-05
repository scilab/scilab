// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 13177 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13177
//
// <-- Short Description -->
// Error when changing x_ticks.locations on axe

clf();
plot(1:10,1:10);
f = gcf();
f.children(1).x_ticks.labels(1) = "99";

clf();
plot(1:10,1:10);
a = gca();
a.x_ticks.labels(1) = "99";