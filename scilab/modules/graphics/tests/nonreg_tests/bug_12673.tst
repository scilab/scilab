// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 12673 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12673
//
// <-- Short Description -->
// Ticks were drawn outside of axes area

clf();
plot([0,1], [0,1]);
a = gca();

// Check that there are no ticks (-0.1 & 1.2) outside the x axis.
a.x_ticks = tlist(["ticks", "locations", "labels"], -0.1:0.2:1.2, string(-0.1:0.2:1.2));