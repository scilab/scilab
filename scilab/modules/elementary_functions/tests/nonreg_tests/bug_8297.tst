// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 8297 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8297
//
// <-- Short Description -->
// After M = rand(m,m);
// cat(3, M, M) spent time was exponential vs m (~12 days for m = 500)

cat(1,1,1);         // To load the function
m = rand(250,250);
tic();
r = cat(3, m, m);
tnew = toc();
assert_checkequal(r(:,:,2), m);

path = "SCI/modules/elementary_functions/tests/nonreg_tests/bug_8297_cat_old.sci";
exec(path,-1);
tic();
r = cat_old(3, m, m);
told = toc();
assert_checktrue(told/tnew > 200);
