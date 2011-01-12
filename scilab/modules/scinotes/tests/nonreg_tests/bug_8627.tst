// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8627 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8627
//
// <-- Short Description -->
// A bad identifier was not shew as invalid.

tmpfile = TMPDIR + "/bug_8627.sce";
mputl(["42identifier = a"; "a != 2"], tmpfile);
scinotes(tmpfile);

// 42identifier should be red with little waves under it.