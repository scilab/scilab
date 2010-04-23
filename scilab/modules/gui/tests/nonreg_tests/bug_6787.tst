// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6787 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6787
//
// <-- Short Description -->
// toprint(filename) required only absolute path to filename.

// To save some paper, this test is interactive

cd(TMPDIR);
mputl("This is a test file","tmp.txt");
if toprint("tmp.txt") <> %t then pause,end
if toprint("tmp2.txt") <> %f then pause,end
ierr = execstr("toprint([''tmp.txt'' ''tmp.txt''])","errcatch");
if ierr <> 999 then pause,end