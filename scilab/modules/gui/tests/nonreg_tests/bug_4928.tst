// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 4928 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4928
//
// <-- Short Description -->
// x_mdialog crashes when more default values than labels

// Execute the following lines into Scilab
rows = ["row1" "row2"];
test = [1 2 3];
answ = x_mdialog("Testing", rows, string(test))

// Press OK: Scilab must not crash
// answ must be equal to ["1","2"]

