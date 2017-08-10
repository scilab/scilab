// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14156 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14156
//
// <-- Short Description -->
// mfscanf returns an empty matrix when datafile contains a header

fid = mopen(fullfile(SCI, "modules", "fileio", "tests", "nonreg_tests", "bug_14156.txt"), 'r');
header=mgetl(fid, 1);
data_all=mfscanf(-1, fid, "%d/%d/%d %d:%d;%lf;%lf;%lf;%lf");
ref = [1 1 1981 0 0 16.84 2.05 10.07 0.5; ..
1 1 1981 1 0 16.9 2.17 10.24 0.5; ..
1 1 1981 2 0 17.47 2.38 10.53 0.5; ..
1 1 1981 3 0 15.9 2.58 10.84 0.5; ..
1 1 1981 4 0 14.8 2.71 11.08 0.5; ..
1 1 1981 5 0 14.54 2.82 11.28 0.5];
assert_checkequal(data_all, ref);
mclose();
