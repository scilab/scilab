// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4878 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4878
//
// <-- Short Description -->
// If a line starts with a comment, "Menu→Edit→Comment Selection" uncomment it.
// 
// A contrario, "Menu→Edit→Uncomment Selection" with a line not commented add a comment.
// 
// => These commands should be totally splitted : "Menu→Edit→Comment Selection" should only comment and "Menu→Edit→Uncomment Selection" should only uncomment.
// => These commands should not have the same shortcut.
// => "Menu→Edit→Comment Selection" should add a second "//" even if the line is already commented.

editor()
// Type the following line : "// A comment"
// Select it
// Menu→Edit→Comment Selection
