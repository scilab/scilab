// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4879 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4879
//
// <-- Short Description -->
// Even if no text is selected, if the edition zone is focused, "Menu→Edit→Comment Region" should comment
// the current line.

editor()
// Type some Scilab code
// Put the cursor wherever you want
// "Menu→Edit→Comment Region"
