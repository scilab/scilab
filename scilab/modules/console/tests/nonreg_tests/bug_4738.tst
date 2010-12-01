// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH CONSOLE -->
//
// <-- Non-regression test for bug 4738 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4738
//
// <-- Short Description -->
// The number of columns in the console was badly calculated.
// Test under Windows

// In the console with default size
1:20

// the matrix should be displayed on 3 lines
// In the font chooser, check "Bold"
1:20

// If all is ok, the two displays of 1:20 should be the same !!



