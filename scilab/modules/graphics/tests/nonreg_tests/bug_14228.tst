// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 14228 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14228
//
// <-- Short Description -->
// Setting rotation_angles property to a matrix of any size does not return an error message 

errmsg = msprintf(_("Wrong size for ''%s'' property: Row vector of size %d expected.\n"), "rotation_angles", 2);
assert_checkerror("plot2d(); a=gca(); a.rotation_angles=[0]", errmsg);
