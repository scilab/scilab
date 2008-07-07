// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3139 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3139
//
// <-- Short Description -->
// In log mode, vertical lines of the grid (just above the tic labels) are missing.

plot([0.01,100],[1,10]);
a           = gca();
a.log_flags = 'lnn';
a.grid      = [12,-1];

// check that all the grid line are drawn


