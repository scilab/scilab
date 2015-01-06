// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11535 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11535
//
// <-- Short Description -->
// listfiles must return absolute filenames if first argument is not a single string

mkdir(TMPDIR + "/bug_11535");
cd(TMPDIR + "/bug_11535");

//create 3 files
mputl("test1.sci", "test1.sci");
mputl("test2.sci", "test2.sci");
mputl("test3.sci", "test3.sci");

//compute reference
filename_ref = ["test3.sci" ; "test2.sci" ; "test1.sci"];
ref = pathconvert(TMPDIR + "/bug_11535") + filename_ref;
ref = gsort([ref;ref]);

//list files in folder
filenames = gsort(listfiles([TMPDIR + "/bug_11535" ; TMPDIR + "/bug_11535/*"]));

//check values
assert_checkequal(filenames, ref);
