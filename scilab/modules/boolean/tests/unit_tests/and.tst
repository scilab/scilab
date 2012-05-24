// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
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