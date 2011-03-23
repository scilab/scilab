// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 4894 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4894
//
// <-- Short Description -->
//    SciNotes should detect when the same file is opened twice.

// scinotes("SCI/etc/scilab.start")

// Wait for end of file loading

// scinotes("SCI/etc/scilab.quit")

// Wait for end of file loading

// scinotes("SCI/etc/scilab.start")

// Wait for end of file loading

// Check that current tab in named "scilab.start"
// Check that scilab.start is opened just once



