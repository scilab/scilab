// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Bruno PINCON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// this is the test matrix from UMFPACK
A = sparse( [ 2  3  0  0  0;
              3  0  4  0  6; 
              0 -1 -3  2  0; 
              0  0  1  0  0; 
              0  4  2  0  1] );
Lup = umf_lufact(A);
[OK, nrow, ncol, lnz, unz, udiag_nz, it] = umf_luinfo(Lup)  // OK must be %t, nrow=ncol = 5, 
[L,U,p,q,R] = umf_luget(Lup);
nnz(L)  // must be equal to lnz
nnz(U)  // must be equal to unz
umf_ludel(Lup) // clear memory
