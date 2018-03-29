// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14487 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14487
//
// <-- Short Description -->
// Matrix indexing is not coherent with MATLAB convention

x=rand(); // scalar
i1=ones(1,4);
i2=ones(2,4);
i3=ones(2,2,2);
assert_checkequal(size(x(i1)), [1 4]);
assert_checkequal(size(x(i1')), [4 1]);
assert_checkequal(size(x(i2)), [2 4]);
assert_checkequal(size(x(i3)), [2 2 2]);

x=rand(1,4); // row vector
i1=ones(1,4);
i2=ones(2,4);
i3=ones(2,2,2);
assert_checkequal(size(x(i1)), [1 4]);
assert_checkequal(size(x(i1')), [1 4]);
assert_checkequal(size(x(i2)), [2 4]);
assert_checkequal(size(x(i3)), [2 2 2]);

x=rand(4,1); // column vector
i1=ones(1,4);
i2=ones(2,4);
i3=ones(2,2,2);
assert_checkequal(size(x(i1)), [4 1]);
assert_checkequal(size(x(i1')), [4 1]);
assert_checkequal(size(x(i2)), [2 4]);
assert_checkequal(size(x(i3)), [2 2 2]);

x=rand(3,3); // matrix
i1=ones(1,4);
i2=ones(2,4);
i3=ones(2,2,2);
assert_checkequal(size(x(i1)), [1 4]);
assert_checkequal(size(x(i1')), [4 1]);
assert_checkequal(size(x(i2)), [2 4]);
assert_checkequal(size(x(i3)), [2 2 2]);

x=rand(3,3,3); // hypermatrix
i1=ones(1,4);
i2=ones(2,4);
i3=ones(2,2,2);
assert_checkequal(size(x(i1)), [1 4]);
assert_checkequal(size(x(i1')), [4 1]);
assert_checkequal(size(x(i2)), [2 4]);
assert_checkequal(size(x(i3)), [2 2 2]);
