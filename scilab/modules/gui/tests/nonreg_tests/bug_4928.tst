// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


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

errmsg = msprintf(_("%s: Wrong size for input argument #%d: It must have same dimensions as argument #%d.\n"), "x_mdialog", 3, 2);

assert_checkerror("x_mdialog(""Testing"", rows, string(test))", errmsg);

// Press OK: Scilab must not crash
// answ must be equal to ["1","2"]

