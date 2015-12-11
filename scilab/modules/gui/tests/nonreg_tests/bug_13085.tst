// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 13085 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13085
//
// <-- Short Description -->
// A datatip cannot be moved on a circle.

t = linspace(0,2*%pi, 400);
clf
plot(1+cos(t),1+sin(t))
a = gca();
a.isoview = "on";
e = gce();
p = e.children;
datatipCreate(p,350)
datatipSetStyle(p,2)

// Now try to move the datatip with the mouse everywhere on the circle.