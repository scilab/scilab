// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 9132 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9132
//
// <-- Short Description -->
// Comments inside a string were removed when executing

f=TMPDIR+"/plop.sce"
mputl(["a=""abcd // efgh"" // ijkl"], f);
scinotes(f,2);

// ctrl+E
// Look at the console
// you should see a="abcd // efgh"
