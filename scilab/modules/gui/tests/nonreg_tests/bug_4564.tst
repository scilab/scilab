// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 4564 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4564
//
// <-- Short Description -->
// Unable to edit matrices using x_mdialog function

// Execute the following lines into Scilab
rows = ["row1" "row2"];
cols = ["col1" "col2" "col3"];
test = [1 2 3;4 5 6];
answ = x_mdialog("Testing", rows, cols, string(test))

// Check that:
// - Row #1 contains: 1 2 3
// - Row #2 contains: 4 5 6
// - col1 label is on top of column containing 1 and 4

// Press OK

// Check the result is OK with the following line
if or(evstr(answ)<>test) then pause;end

