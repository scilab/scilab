// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan SIMON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH EDITOR -->
//
// <-- Non-regression test for bug 5649 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5649
//
// <-- Short Description -->
// The Editor does not undo properly after a find/replace.


//editor(SCI+"\modules\optimization\macros\numderivative.sci")
//* Search> Find/Replace
//Find: Copyright
//Replace With: Copyleft
//* Push "Find"
//* Push "Replace"
//* Push "Close"
// undo
// Copyright should appear again

