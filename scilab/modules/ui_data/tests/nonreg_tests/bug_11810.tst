// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 11810 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11810
//
// <-- Short Description -->
// In EditVar, undesired data were added when the edition is cancelled.

a=1;
editvar("a");

// double-click in the cell 3,3 and use arrow-up key to cancel edition
// nothing must be added in the matrix
