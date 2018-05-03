// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2012 - DIGITEO - Allan CORNET
// Copyright (C) - 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

A = [];
assert_checkequal(and(A), %T);

A = %T(ones(1, 3));
assert_checkequal(and(A), %T);

A = %F(ones(1, 3));
assert_checkequal(and(A), %F);

A = %T(ones(1, 3));
assert_checkequal(and(A, '*'), %T);
assert_checkequal(and(A, 'c'), %T);
assert_checkequal(and(A, 2), %T);

REF = [%T %T %T];
assert_checkequal(and(A, 'r'), REF);
assert_checkequal(and(A, 1), REF);

B = (eye(3,3) == 0);
B(1, 1) = %T;
assert_checkequal(and(B), %F);
assert_checkequal(and(B, '*'), %F);
REF = [%T,%F, %F];
assert_checkequal(and(B, 'r'), REF);
REF = [%T; %F; %F];
assert_checkequal(and(B, 'c'), REF);

// HYPERMATRICES
A = eye(3,3,3);
r = zeros(1,3,3)<0;
c = zeros(3,1,3)<0;
z = zeros(3,3)==1;

// decimal numbers
assert_checkequal(and(A, 'r'), r);
assert_checkequal(and(A, 'c'), c);
assert_checkequal(and(A, 1), r);
assert_checkequal(and(A, 2), c);
assert_checkequal(and(A, 3), z);
// encoded integers
A0 = A;
A = int8(A);
assert_checkequal(and(A, 'r'), r);
assert_checkequal(and(A, 'c'), c);
assert_checkequal(and(A, 1), r);
assert_checkequal(and(A, 2), c);
assert_checkequal(and(A, 3), z);
// Booleans
A = A0==1;
assert_checkequal(and(A, 'r'), r);
assert_checkequal(and(A, 'c'), c);
assert_checkequal(and(A, 1), r);
assert_checkequal(and(A, 2), c);
assert_checkequal(and(A, 3), z);
