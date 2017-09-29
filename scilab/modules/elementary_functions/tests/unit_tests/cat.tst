// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->

// Unit tests for cat.sci function
// ===============================

assert_checkequal(cat(1,[]), []);
assert_checkequal(cat(2,[]), []);
assert_checkequal(cat(3,[]), []);
m = rand(3,4,2);
assert_checkequal(cat(1,m), m);
assert_checkequal(cat(2,m), m);
assert_checkequal(cat(3,m), m);

// first example : concatenation according to the rows
dims = 1; A1=[1 2 3]; A2=[4 5 6 ; 7 8 9]; A3=[10 11 12];
y=cat(dims, A1, A2, A3);
res = [1 2 3; 4 5 6; 7 8 9; 10 11 12];
assert_checkequal(y, res);,
y = cat(dims, A1<5, A2<5, A3<5);
assert_checkequal(y, res<5);,
y = cat(dims, uint8(A1), uint8(A2), uint8(A3));
assert_checkequal(y, uint8(res));,
y = cat(dims, string(A1), string(A2), string(A3));
assert_checkequal(y, string(res));,
A1 = makecell([1 3 2], %pi, %t, "hello", %s, list(-1,"3"), 1/(1-%s));
y = cat(1,A1,A1);
res = makecell([2 3 2], %pi, %t, "hello", ..
                        %pi, %t, "hello", ..
                        %s, list(-1,"3"), 1/(1-%s), ..
                        %s, list(-1,"3"), 1/(1-%s));
assert_checkequal(y, res);


// second example :  concatenation according to the columns
dims = 2; A1=[1 2 3]'; A2=[4 5;7 8;9 10];
y = cat(dims, A1, A2);
res = [1 4 5; 2 7 8; 3 9 10];
assert_checkequal(y, res);,
y = cat(dims, A1<5, A2<5);
assert_checkequal(y, res<5);,
y = cat(dims, uint8(A1), uint8(A2));
assert_checkequal(y, uint8(res));,
y = cat(dims, string(A1), string(A2));
assert_checkequal(y, string(res));,
A1 = makecell([2 2 2], %pi, %t, "hello", %s, list(-1,"3"), 1/(1-%s), "abc", %i);
y = cat(dims, A1, A1);
res = makecell([2 4 2], %pi, %t, %pi, %t, ..
                        "hello", %s, "hello", %s, ..
                        list(-1,"3"), 1/(1-%s), list(-1,"3"), 1/(1-%s), ..
                        "abc", %i, "abc", %i);
assert_checkequal(y, res);

// third example : concatenation according to the 3th dimension
dims = 3; A1=matrix(1:12,[2,2,3]); A2=[13 15;14 16]; A3=matrix(17:32,[2,2,4]);
y=cat(dims, A1, A2, A3);
res = matrix( [1:32], [2, 2, 8] );
assert_checkequal(y, res);
y = cat(dims, A1<5, A2<5, A3<5);
assert_checkequal(y, res<5);,
y = cat(dims, uint8(A1), uint8(A2), uint8(A3));
assert_checkequal(y, uint8(res));,
y = cat(dims, string(A1), string(A2), string(A3));
assert_checkequal(y, string(res));,
A1 = makecell([2 2 2], %pi, %t, "hello", %s, list(-1,"3"), 1/(1-%s), "abc", %i);
y = cat(dims, A1, A1(:,:,1));
res = makecell([2 2 3], %pi, %t, ..
                        "hello", %s,..
                        list(-1,"3"), 1/(1-%s), ..
                        "abc", %i, ..
                        %pi, %t, ..
                        "hello", %s);
assert_checkequal(y, res);

