// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - Bruno PINCON
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

assert_checkfalse(execstr("umf_luget()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "umf_luget", 1);
assert_checkerror("umf_luget()", refMsg);

assert_checkfalse(execstr("umf_luget(1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A pointer expected.\n"), "umf_luget", 1);
assert_checkerror("umf_luget(1)", refMsg);

// this is the test matrix from UMFPACK

A = sparse( [ 2  3  0  0  0;
              3  0  4  0  6; 
              0 -1 -3  2  0; 
              0  0  1  0  0; 
              0  4  2  0  1] );
Lup = umf_lufact(A);
[L,U,p,q,R] = umf_luget(Lup);

assert_checkequal(nnz(L), 9);
assert_checkequal(nnz(U), 9);

assert_checkfalse(or(p <> [ 3; 4; 1; 5; 2 ]));
assert_checkfalse(or(q <> [ 4; 3; 1; 5; 2 ]));
assert_checkfalse(or(R <> [ 5; 13; 6; 1; 7 ]));

B = A;
for i=1:5, B(i,:) = B(i,:)/R(i); end // apply the row scaling
B(p,q) - L*U;  // must be a (quasi) nul matrix

assert_checkequal(nnz(L*U), 12);
assert_checkequal(nnz(B(p,q)), 12);

umf_ludel(Lup); // clear memory

// the same with a complex matrix
A = sparse( [ 2+%i  3+2*%i  0      0    0;
              3-%i  0       4+%i   0    6-3*%i; 
              0    -1+%i   -3+6*%i 2-%i 0; 
              0     0       1-5*%i 0    0; 
              0     4       2-%i   0    1] );
Lup = umf_lufact(A);
[L,U,p,q,R] = umf_luget(Lup);

assert_checkequal(nnz(L), 9);
assert_checkequal(nnz(U), 9);

assert_checkfalse(or(p <> [ 3; 4; 1; 5; 2 ]));
assert_checkfalse(or(q <> [ 4; 3; 1; 5; 2 ]));
assert_checkfalse(or(R <> [ 8; 18; 14; 6; 8 ]));

B = A;
for i=1:5, B(i,:) = B(i,:)/R(i); end // apply the row scaling
B(p,q) - L*U;  // must be a (quasi) nul matrix
assert_checkequal(nnz(B(p,q)), 12);
assert_checkequal(nnz(L*U), 12);

umf_ludel(Lup) // clear memory
