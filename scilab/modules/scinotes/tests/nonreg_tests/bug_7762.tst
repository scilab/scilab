// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 7762 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7762
//
// <-- Short Description -->
// A keyword was not fully helpable depending on the caret position

scinotes(get_function_path("test_run"), 128);

// Put the caret like these "|test_run" or "tes|t_run" or "test_run|"
// In the three cases, in the help menu, we should have "Help on test_run"
