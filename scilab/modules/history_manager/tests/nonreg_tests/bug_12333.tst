//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12333 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12333
//
// <-- Short Description -->
// Cleaned command history is restored after reopening Scilab

// add some history to be sure it is not empty, and save it
addhistory("lineA");
addhistory("lineB");
addhistory("lineC");
addhistory("lineD");
addhistory("lineE");
savehistory();

// reset history, and save it
resethistory();
savehistory();

// reload history => it should have less than 5 lines
loadhistory(SCIHOME + '/history');
h = gethistory();
assert_checktrue(size(h,'r') < 5);
