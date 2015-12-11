// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13132 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13132
//
// <-- Short Description -->
// There were missing graduations when data_bound interval was too small

scf();
plot([0, 1], [0.00099, 0.0010082]);

// Check if all main ticks on y-axis have a label
