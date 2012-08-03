// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9328 -->
//
// <-- CLI SHELL MODE -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9328
//
// <-- Short Description -->
// Scilab randomly crached at startup under Mac OS X when calling
// functions returning no value (lines, modes, banner, ...).

for k=1:1000
    lines(0);
    mode(-1);
end

mprintf("Bug #9328 is fixed.\n");