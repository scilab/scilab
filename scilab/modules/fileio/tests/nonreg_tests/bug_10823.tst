// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 10823 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10823
//
// <-- Short Description -->
// fullpath returns different results on Windows and Linux for non-existent file

cd TMPDIR;
tmp=getlongpathname(TMPDIR);
res1=pathconvert(fullpath("a"));
ref1=pathconvert(tmp+"/a");
assert_checkequal(res1,ref1);
res2=pathconvert(fullpath("a/b"));
ref2=pathconvert(tmp+"/a/b");
assert_checkequal(res2,ref2);
res3=pathconvert(fullpath("a/b/c"));
ref3=pathconvert(tmp + "/a/b/c");
assert_checkequal(res3, ref3);
