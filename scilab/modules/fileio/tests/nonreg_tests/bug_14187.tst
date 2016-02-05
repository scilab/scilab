// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14187 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14187
//
// <-- Short Description -->
// fscanfMat can not read formats %d, %f and %i

fd = mopen(TMPDIR + "/Mat", "w");
mfprintf(fd, "1 2\n");
mfprintf(fd, "3 4\n");
mclose(fd);

assert_checkequal(fscanfMat(TMPDIR + "/Mat"), [1 2; 3 4]);
assert_checkequal(fscanfMat(TMPDIR + "/Mat","%d"), [1 2; 3 4]);
assert_checkequal(fscanfMat(TMPDIR + "/Mat","%i"), [1 2; 3 4]);
assert_checkequal(fscanfMat(TMPDIR + "/Mat","%f"), [1 2; 3 4]);
