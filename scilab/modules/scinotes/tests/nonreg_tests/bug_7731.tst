// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 7731 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7731
//
// <-- Short Description -->
// In Find/Replace window, the buttons were not updated when an item was
// chosen in the combobox Find.

// In SciNotes, launch Find/Replace.
// i) In the field "Find", put foo and search it !
// ii) In the field "Find", remove the contents, normally the buttons
//     are disabled, and select "foo" in the list of the combo, the buttons
//     should be enabled.

