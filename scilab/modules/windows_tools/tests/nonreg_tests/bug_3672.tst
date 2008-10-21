// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3672 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3672
//
// <-- Short Description -->
// When Scilab crashes, a zombie Scilab process (Wscilex.exe) lies in the background. And it is no longer possible to launch Scilab by right clicking on a Scilab script file.

// <-- INTERACTIVE TEST -->

// opens scilab.quit & add a command with some errors
// example : blbalflallafl
// save this file
// launch scilab
// quit scilab
// click on associated file .sci or .sce
// a new scilab will be open
// dont forget to remove line in scilab.quit after ;)

