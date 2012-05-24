// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

//============================================
// external with schur 
// dynamic link test
//============================================
cd TMPDIR;
mkdir('schur_test');
cd('schur_test');
A=diag([-0.9,-2,2,0.9]);X=rand(A);A=inv(X)*A*X;

// The same function in C (a Compiler is required)
C=['int mytest(double *EvR, double *EvI) {' //the C code
   'if (*EvR * *EvR + *EvI * *EvI < 0.9025) return 1;'
   'else return 0; }';];
mputl(C,TMPDIR+'/schur_test/mytest.c');

//build and link
lp=ilib_for_link('mytest','mytest.c',[],'c');
link(lp,'mytest','c'); 

//run it
[U,dim,T]=schur(A,'mytest');

if (dim <> 2) then pause,end;
//============================================
