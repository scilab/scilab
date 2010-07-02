// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5002 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5002
//
// <-- Short Description -->
//   The dialog box when opening a non-existing file doesn't work.
//   The file "xxx" doesn't exist. Do you want to create it. <Cancel> <No> <Yes>
//   If <No> button is pressed, the tab is created anyway.

editor('my_file.sci')






