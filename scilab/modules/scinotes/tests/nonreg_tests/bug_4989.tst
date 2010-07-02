// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 4989 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4989
//
// <-- Short Description -->
//   editor('myfolder/myfile.sci')
//   The correct file is opened. But when I modify and save this file, 
//   it is saved in ./myfile.sci instead of myfolder/myfile.sci

//editor('myfolder/myfile.sci')
// modify and save the file
//file should be saved in 'myfolder'






