// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5007 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5007
//
// <-- Short Description -->
//    When opening a non-existing file, If you answer <yes> to the following dialog box :
//    The file "xxx" doesn't exist. Do you want to create it. <Cancel> <No> <Yes>
//    The file is always saved in SCI instead of the current directory


editor('my_file.sci')






