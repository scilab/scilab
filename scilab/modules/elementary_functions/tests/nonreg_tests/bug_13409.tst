// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug  -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13409
//
// <-- Short Description -->
// permute(x, dims) failed when dims was greater than the dimensions of size(x)
// now permute treats extra dimensions as 1
//
// <-- CLI SHELL MODE -->

x = matrix(1:6, [2, 3]);
// x is of size 2, 2 so has 2 dimension

res = permute(x, [3 2 1]); // this permutes x across the 3rd dimension
// #Explanation
// Dimensions are changed according to dims
// * Number of lines becomes number of pages = 2
// * Number of columns is unchanged = 3
// * Number of pages becomes number of lines = 1
ref = [1 3 5];
ref(:,:,2) = [2 4 6];

assert_checkequal(res, ref);

// Check for extra dimensions
x = matrix(1:12, [3 2 2]);
res = permute(x, [2 3 1 4]);
// Dimensions are changed according to dims
// * Number of lines becomes number of columns = 2
// * Number of columns becomes number of pages = 2
// * Number of pages1 becomes number of lines = 3
// * Number of pages2 becomes number of pages2 = 1
// Extra dimension is squeezed
ref = permute(x, [2 3 1]);
assert_checkequal(res, ref);

x = matrix(1:12, [3 2 2]);
res = permute(x, [3 4 1 2]);
// Dimensions are changed according to dims
// * Number of lines becomes number of pages1 = 2
// * Number of columns becomes number of pages2 = 1
// * Number of pages1 becomes number of lines = 3
// * Number of pages2 becomes number of columns = 2
ref = [1 7]';
ref(:,:,2,1) = [2 8]';
ref(:,:,3,1) = [3 9]';
ref(:,:,1,2) = [4 10]';
ref(:,:,2,2) = [5 11]';
ref(:,:,3,2) = [6 12]';
assert_checkequal(res, ref);

x = matrix(1:12, [3 2 2]);
res = permute(x, 1:10);
// Extra dimensions are squeezed
assert_checkequal(x, res);

// Bug #13409 test case
dims=3; A1=matrix(1:12, [2,2,3]); A2=[13 14;15 16]; A3=matrix(21:36,[2,2,4]); y=cat(dims,A1,A2,A3);
assert_checkequal(A1, y(:,:,1:3));
assert_checkequal(A2, y(:,:,4));
assert_checkequal(A3, y(:,:,5:8));
