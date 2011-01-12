// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8359 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8359
//
// <-- Short Description -->
// An execption was thrown on Save As action

tmpfile = TMPDIR + "/bug_8359.sce";
mputl(["a"], tmpfile);
scinotes(tmpfile);

// Choose Save As action in menu File and check the console. 