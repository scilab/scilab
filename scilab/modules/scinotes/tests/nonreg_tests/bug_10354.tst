// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 10354 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10354
//
// <-- Short Description -->
// Bad caret position after action on selection

mputl('Hello World', TMPDIR + '/bug_10354.sce')
scinotes(TMPDIR + '/bug_10354.sce');

// Select "Hello World" from left to right and CTRL+D, the caret must be at the end
// Select "Hello World" from right to left and CTRL+D, the caret must be at the beginning