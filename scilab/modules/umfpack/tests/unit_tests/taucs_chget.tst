// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - Bruno PINCON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- TEST WITH GRAPHIC -->

// Example #1 : a small linear test system
A = sparse( [ 2 -1  0  0  0;
-1  2 -1  0  0;
0 -1  2 -1  0;
0  0 -1  2 -1;
0  0  0 -1  2] );
Cp = taucs_chfact(A);
[Ct, p] = taucs_chget(Cp);
full(A(p,p) - Ct'*Ct);  // this must be near the null matrix
taucs_chdel(Cp);

// Example #2 a real example
// first load a sparse matrix
[A] = ReadHBSparse(SCI+"/modules/umfpack/demos/bcsstk24.rsa");
// compute the factorization
Cptr = taucs_chfact(A);
// retrieve the factor at scilab level
[Ct, p] = taucs_chget(Cptr);
// plot the initial matrix
xset("window",0) ; clf();
PlotSparse(A) ; xtitle("Initial matrix A (bcsstk24.rsa)");
// plot the permuted matrix
B = A(p,p);
xset("window",1) ; clf();
PlotSparse(B) ; xtitle("Permuted matrix B = A(p,p)");
// plot the upper triangle Ct
xset("window",2) ; clf();
PlotSparse(Ct) ; xtitle("The pattern of Ct (A(p,p) = C*Ct)");
// retrieve cnz
[OK, n, cnz] = taucs_chinfo(Cptr);
// cnz is superior to the realnumber of non zeros elements of C :
cnz_exact = nnz(Ct);
// don't forget to clear memory
taucs_chdel(Cptr);
