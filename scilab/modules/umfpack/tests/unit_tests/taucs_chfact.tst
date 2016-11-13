// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - Bruno PINCON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// Example #1 : a small linear test system 
// whom solution must be [1;2;3;4;5]
A = sparse( [ 2 -1  0  0  0;
             -1  2 -1  0  0; 
              0 -1  2 -1  0; 
              0  0 -1  2 -1; 
              0  0  0 -1  2] );
b = [0 ; 0; 0; 0; 6];
Cp = taucs_chfact(A);
x = taucs_chsolve(Cp,b);
if round(x) <> [ 1; 2; 3; 4; 5 ] then pause, end

// don't forget to clear memory with
taucs_chdel(Cp)

// Example #2 a real example
// first load a sparse matrix
[A] = ReadHBSparse(SCI+"/modules/umfpack/demos/bcsstk24.rsa");
// compute the factorization
Cp = taucs_chfact(A); 
b = rand(size(A,1),1); // a random rhs
// use taucs_chsolve for solving Ax=b
x = taucs_chsolve(Cp,b);
firstNorm=norm(A*x - b);
// the same with one iterative refinement step
x = taucs_chsolve(Cp,b,A);
secondNorm=norm(A*x - b);

if norm(firstNorm,secondNorm) < 1E-7 then pause, end

// don't forget to clear memory
taucs_chdel(Cp)
