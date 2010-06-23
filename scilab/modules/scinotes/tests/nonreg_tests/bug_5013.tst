// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


// <-- TEST WITH SCINOTES -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 5013 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5013
//
// <-- Short Description -->
// Text editor - Find/Replace dialog box : Buttons are resized after the search.


editor(SCI+'/modules/time/macros/datenum.sci')

// CTRL+F
// Enter "datenum" in the find input zone
// Then press the Find button

// → Have a look to buttons, their size must not change






