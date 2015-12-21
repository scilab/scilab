//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// Unit test for Scilab LU

pi=%pi;
i=%i;
e=%e;

a=rand(4,4);b=rand(5,4);ac=a+i*rand(4,4);bc=b+i*rand(5,4);
assert_checktrue(abs((1/a)*a-eye())< 100*%eps);
assert_checktrue(abs((i/a)*a-i*eye())< 100*%eps);
assert_checktrue(abs((1/ac)*ac-eye())< 100*%eps);
assert_checktrue(abs((i/ac)*ac-i*eye())< 100*%eps);
assert_checktrue(abs(a*(a\1)-eye())< 100*%eps);
assert_checktrue(abs(a*(a\i)-i*eye())< 100*%eps);
assert_checktrue(abs(ac*(ac\1)-eye())< 100*%eps);
assert_checktrue(abs(ac*(ac\i)-eye()*i)< 100*%eps);
//
assert_checktrue(abs(inv(a)*a-eye())< 100*%eps);
assert_checktrue(abs(inv(ac)*ac-eye())< 100*%eps);
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
assert_checktrue(abs(inv(h1)-testmatrix('hilb',5))< 1.e-7);
//
assert_checktrue(abs(det(testmatrix('magic',5))-5070000)< 1.e-7);
//
b=a*a';h=chol(b);
assert_checktrue(abs(h'*h-b)< 100*%eps);
bc=triu(ac*ac');bc=bc+bc'-diag(real(diag(bc)));;h=chol(bc);
assert_checktrue(abs(h'*h-bc)< 100*%eps);
