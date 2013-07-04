// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8348 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8348
//
// <-- Short Description -->
// Opening a block parameters without modifying them should not re-position 
// ports.

xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_8348.zcos");
// Open the block settings of the CMSCOPE block.
// OK
// Check that the ports should not be misplaced (check the link)
// Put a SUM_f block on the diagram
// Open the block settings, set the number of port to [1;1;1;1;1;1;1;1;1]
// OK
// Check that the block is not resized.


