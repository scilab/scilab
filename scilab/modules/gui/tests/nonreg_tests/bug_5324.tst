// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 5324 -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5324
//
// <-- Short Description -->
// "File open" dialog box focused on parent directory of current directory, on Windows.
//
// 1) Press "Change Current Directory" icon  in the tool bar and change current directory.
// 2) "File">"Display current directory":Directory which is assigned in step 1) is displayed.
// 3) "File">"Open" :File open dialog MUST SHOW current directory which is assigned in step 1).
 