// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 9129 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9129
//
// <-- Short Description -->
// Completion on field did not propose graphics handle fields by default.

mputl('a.', TMPDIR + '/bug_9129.sce')
scinotes(TMPDIR + '/bug_9129.sce');

// Put the caret after the dot and <CTRL>+<SPACE>
// Graphic handle fields should be proposed