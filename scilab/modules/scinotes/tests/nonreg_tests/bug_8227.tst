// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8227 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8227
//
// <-- Short Description -->
// The commands evaluated with "Evaluate selection" were not stored in history.

tmpfile = TMPDIR+'/bug_8227.sci';
fd = mopen(tmpfile,'wt');
mputl(['a=1','b=2'],fd);
mclose(fd);
editor(tmpfile)

// Select all, right click and evaluate the selection.
// In the console, a pression on <UP> key should show b=2 and a second should
// show a=1

