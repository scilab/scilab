// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6504 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6504
//
// <-- Short Description -->
// It is no more possible in Scilab 5.x to select the content of messageboxes (available in Scilab 4.x)

unix_x("set")

// Try to select, copy and paste the contents of the message box, the lines must be kept.