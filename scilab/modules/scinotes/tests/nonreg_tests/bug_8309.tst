// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8309 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8309
//
// <-- Short Description -->
// Bad display of the line number in whereami mode with broken lines

tmpfile = TMPDIR + "/bug_8309.sce";
mputl(["function toto()", "a=""a ." + ".//a", "b ." + ".//b", "c""", "endfunction"], tmpfile);
scinotes(tmpfile);

// Change the line numbering to be in whereami mode
// All the lines in the body of toto must be numbered 