// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH SCINOTES -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 5460 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5460
//
// <-- Short Description -->
// added menus 'Close All' and Close all but this' in editor.

editor()

// opens some files with editor
// Goto in Editor File --> Close All
// it should close all files and opens a untitled

// opens some files with editor
// Select a file
// Goto in Editor File --> Close All But This
// it should close all file except selected file




