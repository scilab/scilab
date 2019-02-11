// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// Unit test for Scilab LU

pi=%pi;
I=eye(4,4);
IC=%i*I;
e=%e;

a=rand(4,4);b=rand(5,4);ac=a+%i*rand(4,4);bc=b+%i*rand(5,4);
assert_checktrue(abs((I/a)*a-I)< 100*%eps);
assert_checktrue(abs((IC/a)*a-IC)< 100*%eps);
assert_checktrue(abs((I/ac)*ac-I)< 100*%eps);
assert_checktrue(abs((IC/ac)*ac-IC)< 100*%eps);
assert_checktrue(abs(a*(a\I)-I)< 100*%eps);
assert_checktrue(abs(a*(a\IC)-IC)< 100*%eps);
assert_checktrue(abs(ac*(ac\I)-I)< 100*%eps);
assert_checktrue(abs(ac*(ac\IC)-IC)< 100*%eps);
//
assert_checktrue(abs(inv(a)*a-I)< 100*%eps);
assert_checktrue(abs(inv(ac)*ac-I)< 100*%eps);
//
assert_checktrue(abs((b/a)*a-b)< 100*%eps);
assert_checktrue(abs((b/ac)*ac-b)< 100*%eps);
assert_checktrue(abs((bc/a)*a-bc)< 100*%eps);
assert_checktrue(abs((bc/ac)*ac-bc)< 100*%eps);
//
assert_checktrue(abs(a*(a\b')-b')< 100*%eps);
assert_checktrue(abs(ac*(ac\b')-b')< 100*%eps);
assert_checktrue(abs(a*(a\bc')-bc')< 100*%eps);
assert_checktrue(abs(ac*(ac\bc')-bc')< 100*%eps);
//
[l u]=lu(a);
assert_checktrue(abs(l*u-a)< 100*%eps);
[l u]=lu(ac);
assert_checktrue(abs(l*u-ac)< 100*%eps);
//
h1(5,5)=0;for k=1:5,for l=1:5, h1(k,l)=1/(k+l-1);end;end;
assert_checkalmostequal(inv(h1),testmatrix("hilb",5));
//
assert_checktrue(abs(det(testmatrix("magic",5))-5070000)< 1.e-7);
//
b=a*a';h=chol(b);
assert_checkalmostequal(h'*h, b, [], 100*%eps);
bc=triu(ac*ac');bc=bc+bc'-diag(real(diag(bc)));;h=chol(bc);
assert_checkalmostequal(h'*h, bc, [], 100*%eps);
