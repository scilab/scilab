// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13127 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13127
//
// <-- Short Description -->
// There were no subticks with user defined ticks

clf, plot([1.2345601 1.2345602],[1.2345601 1.2345602])
ax = gca();

// Check that there are subticks on x-axis
ax.x_ticks.labels=["A" "B" "C"]; 