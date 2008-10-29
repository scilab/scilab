// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3702 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3702
//
// <-- Short Description -->
// When I start Scilex (on Windows) in a windows console, the windows lost the focus. I must hit <alt>+<tab> to gain the focus.

// <-- INTERACTIVE TEST -->
// On Windows
// launch a shell (cmd.exe)
// go to in SCI/bin
// scilex.exe
// try to type : 1+1
// if you need to click on console this test is failed