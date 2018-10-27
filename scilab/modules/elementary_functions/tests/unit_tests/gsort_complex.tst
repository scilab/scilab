// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) ????-2008 - ENPC
// Copyright (C) 2008 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//================================ sparse complex ==============================

// Tests in gsort_sparse.tst

//================================ complex matrix ==============================
x = [3  1  5 ; 2 9 8];
y = [2  4  1 ; 4 1 3];
c = x + y * %i;

ref_ind = [    4.    5.    3.  ;    6.    2.    1.  ];
ref_values = [     9. + %i      5. + %i      1. + 4.*%i  ;    8. + 3.*%i    2. + 4.*%i    3. + 2.*%i ];
[v,ind] = gsort(c);
assert_checkequal(ref_ind, ind);
assert_checkequal(ref_values, v);

[v1,ind1] = gsort(abs(c));
[v2,ind2] = gsort(c);
assert_checkequal(ind1, ind2);

A = [18 21 10 7 5];
B = [1  3  22 8 4];
y = complex(A,B);
[a,b] = gsort(y);
assert_checkequal(b, [3 2 1 4 5]);
assert_checkequal(y(b), a);

assert_checkfalse(execstr("[a,b] = gsort(y,''l'')", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: [''g'' ''r'' ''c'' ''lc'' ''lr''] expected.\n"), "gsort", 2);
assert_checkerror("[a,b] = gsort(y,''l'')", refMsg);

ierr = execstr("[a,b] = gsort(y,''g'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''r'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''c'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''lc'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''lr'');","errcatch");
assert_checkequal(ierr, 0);
