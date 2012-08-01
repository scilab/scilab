// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- WINDOWS ONLY -->
//
// <-- Non-regression test for bug 11578 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11578
//
// <-- Short Description -->
// dir function under windows taking into account the case to sort the results.

mkdir(TMPDIR + "/bug_11578");
cd(TMPDIR + "/bug_11578");

//create 3 files
mputl("a", "a.sci");
mputl("B", "B.sci");
mputl("c", "c.sci");
mputl("D", "D.sci");

ref = ["a.sci"; "B.sci"; "c.sci"; "D.sci"];

filenames = dir(TMPDIR + "/bug_11578");

//check values
assert_checkequal(filenames.name, ref);
