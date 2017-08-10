//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA Michael Baudin
// Copyright (C) 2015 - Scilab Enterprises - John Gliksberg
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==========================================================================
//==============================   det        ==============================
//==========================================================================

//Small dimension
//Real
A=[1 1; 1 2];
assert_checkalmostequal(det(A), 1);
[e,m]=det(A);
assert_checkalmostequal(e, 0);
assert_checkalmostequal(m, 1);
//Complex
A=A+%i;
assert_checkalmostequal(real(det(A)), 1);
assert_checkalmostequal(imag(det(A)), 1);
[e,m]=det(A);
assert_checkalmostequal(e, 0);
assert_checkalmostequal(real(m), 1);
assert_checkalmostequal(imag(m), 1);
//Sparse
A=[1 1; 1 2];
A=sparse(A);
assert_checkalmostequal(det(A), 1);
[e,m]=det(A);
assert_checkalmostequal(e, 0);
assert_checkalmostequal(m, 1);
//Polynomials
A=[1+%s 1; 1 2+%s];
assert_checkequal(det(A), 1+3*%s+%s*%s);
//Rationals
A=[1+%s 1/%s; 1 2+%s];
assert_checkequal(numer(det(A)), -1+2*%s+3*%s^2+%s^3);
assert_checkequal(denom(det(A)), %s);
//Sparse complex
A=[1 1; 1 2];
A=A+%i;
A=sparse(A);
assert_checkalmostequal(real(det(A)), 1);
assert_checkalmostequal(imag(det(A)), 1);
[e,m]=det(A);
assert_checkalmostequal(e, 0);
assert_checkalmostequal(real(m), 1);
assert_checkalmostequal(imag(m), 1);

//Large dimension
//Real
v=rand(1,21);
A=rand(21,21); A=(triu(A,1)+diag(v))*(tril(A,-1)+diag(ones(1,21)));
assert_checktrue(abs(det(A) - prod(v)) < 1D-7);
[e,m]=det(A);
assert_checktrue(abs(m*(10^e) - prod(v)) < 1D-7);
//Complex
v=(v+rand(v)*%i)/2;
A=rand(21,21); A=(triu(A,1)+diag(v))*(tril(A,-1)+diag(ones(1,21)));
assert_checktrue(abs(det(A) - prod(v)) < 1D-7);
[e,m]=det(A);
assert_checktrue(abs(m*(10^e) - prod(v)) < 1D-7);
//Sparse
v=rand(1,21);
v=sparse(v);
A=rand(21,21); A=(triu(A,1)+diag(v))*(tril(A,-1)+diag(ones(1,21)));
assert_checktrue(abs(det(A) - prod(v)) < 1D-7);
[e,m]=det(A);
assert_checktrue(abs(m*(10^e) - prod(v)) < 1d-7);
//Polynomials
v=rand(1,21);
v=v+%s;
A=rand(21,21); A=(triu(A,1)+diag(v))*(tril(A,-1)+diag(ones(1,21)));
assert_checktrue(abs(coeff(det(A)-prod(v))) < 1D-7);
//Rationals
v=rand(1,21);
v=v/%s;
A=rand(21,21); A=(triu(A,1)+diag(v))*(tril(A,-1)+diag(ones(1,21)));
assert_checktrue(abs(coeff(numer(det(A))-numer(prod(v)))) < 1D-7);
//Sparse complex
v=rand(1,21);
v=(v+rand(v)*%i)/2;
v=sparse(v);
A=rand(21,21);
A=(triu(A,1)+diag(v))*(tril(A,-1)+diag(ones(1,21)));
A=sparse(A);
assert_checktrue(abs(det(A) - prod(v)) < 1D-7);
[e,m]=det(A);
assert_checktrue(abs(m*(10^e) - prod(v)) < 1d-7);

//Error messages
A=[1 1; 1 2];
errmsg1 = msprintf(_("%s: Wrong type for input argument #%d: Square matrix expected.\n"), "det", 1);
assert_checkerror("det([1,2;3,4;5,6])", errmsg1, 20);
errmsg2 = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "det", 1);
assert_checkerror("det(A,1)", errmsg2, 77);

// Check det == 0 for simple cases
A = 0;
assert_checkalmostequal(det(A), 0);
A = [1 2
1 2];
assert_checkalmostequal(det(A), 0);
A = [1 2 3
1 2 3
1 2 3];
assert_checkalmostequal(det(A), 0);
b = rand(1, 5);
A = [b
b
b
b
b];
assert_checkalmostequal(det(A), 0);

