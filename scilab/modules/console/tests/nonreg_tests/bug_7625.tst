// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH CONSOLE -->
//
// <-- Non-regression test for bug 7625 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7625
//
// <-- Short Description -->
// Problem with selection which disappeared when a window lost the focus.

// i) Have a terminal and a console opened.
// ii) Select a line in the console.
// iii) Select a line in the terminal.

// the selection in the console shouldn't disappear.
