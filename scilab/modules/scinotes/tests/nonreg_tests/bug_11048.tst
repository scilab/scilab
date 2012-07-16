// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 11048 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11048
//
// <-- Short Description -->
// 

mputl('a=1', TMPDIR + '/bug_11048.sce')
scinotes(TMPDIR + '/bug_11048.sce');

// Put the caret before a
// CTRL+D to comment
// CTRL+SHIFT+D to uncomment
// Move the caret after =
// CTRL+D to comment
// CTRL+SHIFT+D to uncomment
