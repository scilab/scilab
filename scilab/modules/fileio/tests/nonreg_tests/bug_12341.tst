// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 12341 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12341
//
// <-- Short Description -->
// rmdir() could delete a branch in which pwd() is

mkdir(TMPDIR+"/foo");
mkdir(TMPDIR+"/foo/bar");
cd(TMPDIR+"/foo/bar");
s = rmdir(TMPDIR+"/foo");
assert_checkequal(s, 0);
s = rmdir(TMPDIR+"/foo/bar");
assert_checkequal(s, 0);
