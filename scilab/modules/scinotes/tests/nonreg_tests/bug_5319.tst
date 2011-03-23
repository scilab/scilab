// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH SCINOTES -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 5319 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5319
//
// <-- Short Description -->
// The 'Save as' dialog box should set as initial (default) name the present file name (instead of blank), whenever the file to be saved is already named.

editor("SCI/modules/dynamic_link/macros/haveacompiler.sci")
// File -> Save As
// Check the default file name proposed is 'haveacompiler.sci'





