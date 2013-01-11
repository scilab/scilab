// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12168 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12168
//
// <-- Short Description -->
// Segfault when listing a closed matfile.


A = rand(10,10);
testfile = fullfile(TMPDIR, "bug_12168.mat");
savematfile(testfile, "A", "-v6");
fd = matfile_open(testfile);
matfile_close(fd);
refMsg = msprintf(_("%s: Invalid file identifier.\n"), "matfile_listvar");
assert_checkerror("matfile_listvar(fd);", refMsg);
