// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14059 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14059
//
// <-- Short Description -->
// Lack of performance on vector write operation
// before patch first test take around 300 seconds
// with patch it take < 0.1 second on the same machine.

r=rand(1,1000000);
tic();r(r<0.5) = [];t = toc();
assert_checktrue(t < 1);

r=rand(1,1000000);
tic();r(r<0.1) = [];t = toc();
assert_checktrue(t < 1);

r=rand(1,1000000);
tic();r(r<0.9) = [];t = toc();
assert_checktrue(t < 1);



