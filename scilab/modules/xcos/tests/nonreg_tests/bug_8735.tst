// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8735 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8735
//
// <-- Short Description -->
// Mixed (Implicit-Explicit) superblocks I/O blocks produced warnings

xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_8735.zcos");
// Open the super-block, check that it does not contains errors

