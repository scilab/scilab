// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8085 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8085
//
// <-- Short Description -->
// It was not possible to disable the horizontal line wrapping.

edit('test_run')

// Go to the menu 'Preference' and uncheck the menu Horizontal wrapping,
// an horizontal scrollbar should appear and the lines of the document
// should not be wrapped.
