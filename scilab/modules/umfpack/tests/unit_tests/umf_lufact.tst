// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - Bruno PINCON
// Copyrigth (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

assert_checkfalse(execstr("umf_lufact()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "umf_lufact", 1);
assert_checkerror("umf_lufact()", refMsg);

assert_checkfalse(execstr("umf_lufact(1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A sparse matrix expected.\n"), "umf_lufact", 1);
assert_checkerror("umf_lufact(1)", refMsg);

// this is the small linear test system from UMFPACK
// whom solution must be [1;2;3;4;5]
A = sparse( [ 2  3  0  0  0;
              3  0  4  0  6;
              0 -1 -3  2  0;
              0  0  1  0  0;
              0  4  2  0  1] );
b = [8 ; 45; -3; 3; 19];
Lup = umf_lufact(A);
x = umf_lusolve(Lup,b);

assert_checkalmostequal(x, [1 2 3 4 5]');

// solve now A'x=b
x = umf_lusolve(Lup,b,"A''x=b");
assert_checkequal(norm(A'*x-b), 0)

// don't forget to clear memory with
umf_ludel(Lup)

// a real (but small)  example
// first load a sparse matrix
[A] = ReadHBSparse(SCI+"/modules/umfpack/demos/arc130.rua");
// compute the factorization
Lup = umf_lufact(A);
b = rand(size(A,1),1); // a random rhs
// use umf_lusolve for solving Ax=b
x = umf_lusolve(Lup,b);
firstNorm=norm(A*x - b);

// now the same thing with iterative refiment
x = umf_lusolve(Lup,b,"Ax=b",A);
secondNorm=norm(A*x - b);

assert_checkfalse(firstNorm <> secondNorm);

// don't forget to clear memory
umf_ludel(Lup)
