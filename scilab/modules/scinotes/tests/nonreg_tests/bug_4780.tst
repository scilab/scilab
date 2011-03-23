// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4780 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4780
//
// <-- Short Description -->
// The insertion point goes to the end of the document at each newly entered char.
// Once unchecked The Document/Colorize menu item is impossible to check again.
// If there is no selected line and one execute the Execute/EvaluateSelection menu a java exeception is reported.

editor()

// In colorize and autoindent mode and an empty new document
// if one first writes:
// for k=1:3
// a=k
// end
// Now try to append a line after the instruction a=k

// Menu : Document -> Colorize
// No more check option for Colorize instruction

// Menu : Execute -> EvaluateSelection
// Without selecting any line
// With selecting line(s)
