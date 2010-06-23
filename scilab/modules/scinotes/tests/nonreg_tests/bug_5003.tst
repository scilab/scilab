// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5003 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5003
//
// <-- Short Description -->
//   When opening a non-existing file, MSG: "The file "xxx" doesn't exist. Do you want to create it. <Cancel> <No> <Yes>"
//   If <Yes> is pressed, 2 tabs with the same name are created.

// No more 2 tabs with the same name
editor('my_file.sci')






