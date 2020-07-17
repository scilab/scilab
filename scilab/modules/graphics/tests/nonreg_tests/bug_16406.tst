// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 16406 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16406
//
// <-- Short Description -->
// edit_curv() yielded an error when reading data

edit_curv([1 2 3 4],[0 2 1 3],"xya")

// Select the Data => Save action
// Save the current polyline in test_16406.xy
// Select the Data => Clear action
// Select the Data => Read action, and select the test_16406.xy file in the current directory

// CHECK :
//  - The polyline must be displayed
//  - no error message in the console

// Close the edit_curv GUI

mdelete("test_16406.xy");
