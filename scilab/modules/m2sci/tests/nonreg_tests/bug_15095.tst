// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- WINDOWS ONLY -->
//    Extension to Linux and MacOS to come later
// <-- Non-regression test for bug 15095 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15095
//
// <-- Short Description -->
// mfile2sci() and translatepaths() failed when working with non-ASCII paths

cd(TMPDIR);
Dir = TMPDIR + filesep() + "bug_15095 àé" + filesep();
File = Dir + "ìô.m";
mkdir(Dir);
mputl("a = rand(2,3);", File);
assert_checkequal(execstr("mfile2sci(File, verbose_mode=0)", "errcatch"), 0);
assert_checkequal(execstr("mfile2sci(File, Dir, verbose_mode=0)", "errcatch"), 0);
assert_checkequal(execstr("translatepaths(Dir)", "errcatch"), 0);
assert_checkequal(execstr("translatepaths(Dir, Dir)", "errcatch"), 0);

rmdir(Dir, "s")
