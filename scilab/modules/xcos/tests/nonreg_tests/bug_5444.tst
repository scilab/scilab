// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 5444 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5444
//
// <-- Short Description -->
// Error report on superblock diagram is not supported.

xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_5444.zcos");

// start simulation
// After the first error message, check that superblock is highlighted and in the new window, MUX and DEMUX blocks are highlighted too.
