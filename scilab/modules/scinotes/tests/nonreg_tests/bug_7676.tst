// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 7676 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7676
//
// <-- Short Description -->
// The SaveAs action didn't remove the readonly mode

// In the console:
// scinotes(get_function_path("test_run"), "readonly")

// In SciNotes, choose "Save As" in the menu "File" and save it as
// you want, the file is now editable.
