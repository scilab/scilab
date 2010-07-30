// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5001 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5001
//
// <-- Short Description -->
//   The dialog box when opening a non-existing file is not the right one :
//   The file "xxx" doesn't exist. Do you want to create it. <Cancel> <No> <Yes>
//   Only <no> & <yes> buttons should appear. <Cancel> button is unnecessary.

editor('my_file.sci')






