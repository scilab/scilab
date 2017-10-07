// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8669 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8669
//
// <-- Short Description -->
// Some insertions as an hypermatrix page failed
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

A = zeros(3,3);
B = ones(3,3);

assert_checkequal(execstr("A(:,:,:) = B", "errcatch"), 0);
assert_checkequal(execstr("A(:,:,:) = B(:,:,:)", "errcatch"), 0);
assert_checkequal(execstr("A(:,1,:) = B(1,:,:)", "errcatch"), 0);
