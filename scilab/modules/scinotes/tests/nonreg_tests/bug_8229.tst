// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8229 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8229
//
// <-- Short Description -->
// Problem of selection after an indentation

tmpfile = TMPDIR + "/bug_8229.sce";
mputl(["a";"";"b"], tmpfile);
scinotes(tmpfile);

// select the lines 2 and 3, ctrl+I to reindent, the lines 2 and 3 must be selected
