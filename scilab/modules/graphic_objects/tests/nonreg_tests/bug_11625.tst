// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Caio SOUZA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 11625 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11625
//
// <-- Short Description -->
// uicontrol table did not update its object string, the graphic window show the new
// string but the command line property shows the old string.

// Example:
clf, f = gcf(); as = f.axes_size;
table = string(rand(3,3))
ut = uicontrol("style","table",..
"string",table,..
"position",[5 as(2)-100 300 60]);

// Then, modify some values in the table. Then:
t2 = matrix(ut.string, size(table))
// The initial table should have its strings updated to the new ones, enterend in the graphic window.
