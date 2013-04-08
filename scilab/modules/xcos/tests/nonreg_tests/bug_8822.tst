// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8822 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8822
//
// <-- Short Description -->
// Implicit links connected with split blocks might not be removed cleanly.

xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_8822.zcos");
// delete the link tagged with "please delete" (from #3)
// check that the link #1->#2 is not deleted

