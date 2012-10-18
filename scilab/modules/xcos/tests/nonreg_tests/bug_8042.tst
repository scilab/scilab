// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8042 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8042
//
// <-- Short Description -->
// Port ordering is not the good one when using region to super block

xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_8042.xcos");

// Select the middle blocks (below the green rectangle)
// right-click -> region to superblock
// None of the reconnected links should be crossed in the Superblock diagram
// nor in the parent diagram

