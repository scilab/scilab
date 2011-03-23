// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5131 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5131
//
// <-- Short Description -->
//   editor('') opens a non-existing file which name is the current directory base-name.

editor("");
// Check that you get an error message and editor is opened with a new Untitled file

editor(SCI);
// Check that you get an error message and editor is opened with a new Untitled file
