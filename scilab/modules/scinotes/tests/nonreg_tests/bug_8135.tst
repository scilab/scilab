// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8135 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8135
//
// <-- Short Description -->
// An exception was thrown when restoring files on MacOS X.

edit test_run;
closeEditor;
edit test_run;

// Check in the terminal if an exception is thrown.
