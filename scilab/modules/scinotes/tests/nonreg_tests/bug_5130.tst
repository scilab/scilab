// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5130 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5130
//
// <-- Short Description -->
//   No warning is displayed when "Save As" command failed (because you have no write access on the directory for example) and an exception is printed in the terminal.

editor()
// Write some characters
// CTRL+W
// Try save the file in a directory where you do not have write access to.
                    