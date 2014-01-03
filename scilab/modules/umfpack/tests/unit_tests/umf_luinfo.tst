// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Bruno PINCON
// Copyright (C) 2013 -  Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

assert_checkfalse(execstr("umf_luinfo()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "umf_luinfo", 1);
assert_checkerror("umf_luinfo()", refMsg);

assert_checkfalse(execstr("umf_luinfo(1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A pointer expected.\n"), "umf_luinfo", 1);
assert_checkerror("umf_luinfo(1)", refMsg);

// this is the test matrix from UMFPACK
A = sparse( [ 2  3  0  0  0;
              3  0  4  0  6; 
              0 -1 -3  2  0; 
              0  0  1  0  0; 
              0  4  2  0  1] );
Lup = umf_lufact(A);
[OK, nrow, ncol, lnz, unz, udiag_nz, it] = umf_luinfo(Lup);  // OK must be %t, nrow=ncol = 5, 

assert_checkequal(OK, %t);
assert_checkequal(nrow, 5);
assert_checkequal(ncol, 5);
assert_checkequal(lnz, 9);
assert_checkequal(unz, 9);
assert_checkequal(udiag_nz, 5);
assert_checkequal(it, 0);

[L,U,p,q,R] = umf_luget(Lup);
assert_checkequal(nnz(L), lnz);
assert_checkequal(nnz(U), unz);
umf_ludel(Lup) // clear memory
