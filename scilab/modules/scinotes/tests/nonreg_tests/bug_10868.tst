// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 10868 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10868
//
// <-- Short Description -->
// 

mputl('xtitle(""TITLE"", ""$LaTeXed a^b$"", ""y"")', TMPDIR + '/bug_10868.sce')
scinotes(TMPDIR + '/bug_10868.sce');

// Select the string and execute the selection with echo (CTRL+E)
// The string should appear in the console and executed.