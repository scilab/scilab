// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13238 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13238
//
// <-- Short Description -->
// Wrong legends display

t = 0:0.1:2*%pi;
plot2d(t, [cos(t'), cos(2*t'), cos(3*t')], [-1 2 3]);

// Check that the legend is drawn in lower right corner
legends(["cos(t)" ; "cos(2*t)" ; "cos(3*t)"], [-1 2 3],opt = "lr");