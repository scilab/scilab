// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 126 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=126
//
// <-- Short Description -->
//    x_message() does not accept the second argument (list of buttons).  With only one argument, the string is displayed correctly, but x_message() does not return a value indicating which button ("OK" or "Cancel") was pressed.

// <-- INTERACTIVE TEST -->

// 2009-01-09 - x_message(obsolete) replaced by messagebox.

messagebox(["Hello"])

r = messagebox(["Your problem is ill conditioned";"continue?"],"modal","question",["Yes","No"])
