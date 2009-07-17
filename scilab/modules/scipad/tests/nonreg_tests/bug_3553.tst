// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 3553 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3553
//
// <-- Short Description -->
//    system crash executing tk_getfile() from within scipad by CTRL+L.

// 1 - Start Scilab

// 2 - Start Scipad

// 3 - In Scipad enter:
// tk_getfile()

// 4 - CTRL+L

// 5 - Check that you can select a file
