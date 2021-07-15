// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA Michael Baudin
// Copyright (C) 2015 - Scilab Enterprises - John Gliksberg
// Copyright (C) 2021 - Le Mans Université - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

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
assert_checkequal(det(A).num, -1+2*%s+3*%s^2+%s^3);
assert_checkequal(det(A).den, %s);
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
assert_checktrue(abs(coeff(det(A).num - prod(v).num)) < 1D-7);
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
assert_checkalmostequal(det(A), 0, %eps, norm(A)*%eps);

a = sprand(1000,1000,0.1); a(:,1:2) = 1;
assert_checkequal(det(a), 0);

// Underflow and overflow management for sparse matrices. [e, m] = det(s)  syntax
// -------------------------------------------------------------------------------
// http://bugzilla.scilab.org/16636 :
// With a real matrix
// ..................
// Underflow:
n = 75;
while n < 3000
    s = sparse(triu(rand(n,n)));
    ref = prod(diag(s));
    if n < 1000
        assert_checkalmostequal(det(s), ref, 10*n*%eps);
    else
        assert_checkequal(det(s), 0);
    end
    [e, m] = det(s);
    ref = sum(log10(full(diag(s))));
    assert_checkequal(e, int(ref)-1);
    assert_checkalmostequal(m, 10^(ref-int(ref)+1), 10*n*%eps);
    n = n*2;
end
// Overflow:
n = 75;
while n < 3000
    s = sparse(triu(rand(n,n)))*6;
    ref = prod(diag(s));
    if n < 1000
        assert_checkalmostequal(det(s), ref, 10*n*%eps);
    else
        assert_checkequal(det(s), %inf);
    end
    [e, m] = det(s);
    ref = sum(log10(full(diag(s))));
    assert_checkequal(e, int(ref));
    assert_checkalmostequal(m, 10^(ref-int(ref)), 10*n*%eps);
    n = n*2;
end
// With a complex matrix
// .....................
// Underflow:
n = 75;
while n < 3000
    s = sparse(triu(complex(rand(n,n),rand(n,n))));
    ref = prod(diag(s));
    if n < 2000
        assert_checkalmostequal(det(s), ref, 100*n*%eps);
    else
        assert_checkequal(det(s), 0*%i);
    end
    [e, m] = det(s);
    [eref, mref] = det(full(s));
    assert_checkequal(e, eref);
    assert_checkalmostequal(m, mref, 100*n*%eps);
    n = n*2;
end
// Overflow:
n = 75;
while n < 3000
    s = sparse(triu(complex(rand(n,n),rand(n,n))))*6;
    ref = prod(diag(s));
    if n < 600
        assert_checkalmostequal(det(s), ref, 100*n*%eps);
    else
        assert_checkequal(abs(real(det(s))), %inf);
    end
    [e, m] = det(s);
    [eref, mref] = det(full(s));
    assert_checkequal(e, eref);
    assert_checkalmostequal(m, mref, 200*n*%eps);
    n = n*2;
end
