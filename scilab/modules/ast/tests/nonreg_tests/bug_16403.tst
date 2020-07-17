// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16144 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16403
//
// <-- Short Description -->
// 1D extraction of matrix with implicit index has wrong dimensions

a = [1 3 5
     2 4 6];
k = 1:3;
assert_checkequal(a(1:3),[1, 2, 3]);
assert_checkequal(a(k),[1, 2, 3]);
assert_checkequal(a(k'),[1; 2; 3]);

// extracted vectors keep the shape of source
x=[1, 2, 3, 4, 5, 6];
assert_checkequal(x(1:3),[1, 2, 3]);
assert_checkequal(x(k),[1, 2, 3]);
assert_checkequal(x(k'),[1, 2, 3]);
x=[1; 2; 3; 4; 5; 6];
assert_checkequal(x(1:3),[1; 2; 3]);
assert_checkequal(x(k),[1; 2; 3]);
assert_checkequal(x(k'),[1; 2; 3]);
