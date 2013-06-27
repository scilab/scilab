// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 3928 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3928
//
// <-- Short Description -->
// Error when a matrix is flipped along the third dimension

x=matrix(1:48,[3 2,4,2]);
dim=3;
y=flipdim(x,dim);
assert_checkequal(y(:,:,1,1), x(:,:,4,1));
assert_checkequal(y(:,:,2,1), x(:,:,3,1));
assert_checkequal(y(:,:,3,1), x(:,:,2,1));
assert_checkequal(y(:,:,4,1), x(:,:,1,1));
assert_checkequal(y(:,:,1,2), x(:,:,4,2));
assert_checkequal(y(:,:,2,2), x(:,:,3,2));
assert_checkequal(y(:,:,3,2), x(:,:,2,2));
assert_checkequal(y(:,:,4,2), x(:,:,1,2));
