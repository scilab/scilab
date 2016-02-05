//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// Unit test for Scilab scalars, vectors and matrix

pi=%pi;
i=%i;
e=%e;

// .^ TEST WITH  REAL VECTORS

x1=1:3;
x0=0:2;
assert_checkequal(x1.^2,[1 4 9]);
assert_checkequal(x0.^2,[0 1 4]);
assert_checkequal(x1.^0,[1 1 1]);

assert_checktrue(norm(x1.^(2*(1+%eps))-x1.^2)<100*%eps);
assert_checktrue(norm(x0.^(2*(1+%eps))-x0.^2)<100*%eps);

assert_checktrue(norm(x1.^(-2)-[1 0.25 1/9])<100*%eps);
assert_checktrue(norm(x1.^(-2*(1+%eps))-[1 0.25 1/9])<100*%eps);

// COMPLEX CASE
p=2+%eps*%i;
assert_checktrue(norm(x1.^p-x1.^2)<100*%eps);
assert_checktrue(norm(x0.^p-x0.^2)<100*%eps);
assert_checktrue(norm(x1.^(-p)-x1.^(-2))<100*%eps);

y=%eps*%eps*ones(1,3);x1=x1+y;x0=x0+y;
assert_checktrue(norm(x1.^2-[1 4 9])<100*%eps);
assert_checktrue(norm(x0.^2-[0 1 4])<100*%eps);

x1.^2.000000001;x0.^2.000000001;
assert_checktrue(norm(x1.^2-[1 4 9])<100*%eps);
assert_checktrue(norm(x0.^2-[0 1 4])<100*%eps);

assert_checktrue(norm(x1.^(-2)-[1 0.25 1/9])<100*%eps);
assert_checktrue(norm(x1.^(-2*(1+%eps))-[1 0.25 1/9])<100*%eps);
assert_checktrue(norm(x1.^p-x1.^2)<100*%eps);
assert_checktrue(norm(x0.^p-x0.^2)<100*%eps);
assert_checktrue(norm(x1.^(-p)-x1.^(-2))<100*%eps);
//
// EMPTY MATRIX
//=============
assert_checkequal([].^1,[]);
assert_checkequal([].^[1 2 3],[]);
assert_checkequal([1 2 3].^[],[]);
assert_checkequal([].^[],[]);

// SQUARE MATRIX
//==============

x1=[1 2;3 4];
assert_checkequal(x1^1,x1);
assert_checkequal(x1^(-1),inv(x1));
assert_checkequal(x1^2,x1*x1);
assert_checkalmostequal(x1^(-2),inv(x1)^2);

x1(1,1)=x1(1,1)+%i;
assert_checkequal(x1^2, x1*x1);
assert_checkalmostequal(x1^(-2), inv(x1)^2);
assert_checkequal(x1^1, x1);
assert_checkequal(x1^(-1), inv(x1));

assert_checktrue(rand(4,4)^0==eye(4,4));


x1=[1 2;2 3];x2=x1^(1/2);
assert_checktrue(norm(x2^(2)-x1)<100*%eps);
x2=x1^(-1/2);
assert_checktrue(norm(x2^(-2)-x1)<100*%eps);

//TEST WITH POLYNOMIAL VECTOR AND MATRICES
//---------------------------------------
s=poly(0,"s");
assert_checkequal(coeff(s^3+1),[1 0 0 1]);

x1=[1 s+1 s.^3+1];
assert_checkequal(x1.^2,[1 1+2*s+s.^2  1+2*s.^3+s.^6]);
assert_checkequal(coeff(x1.^0),[1 1 1]);
assert_checkequal(x1.^3,[1,1+3*s+3*s^2+s^3,1+3*s^3+3*s^6+s^9]);
assert_checkequal(coeff(numer((x1.^(-1)-[1 1/(1+s)  1/(1+s.^3)]))),[0 0 0]);


x1=[s+1 2*s;3+4*s^2 4];
assert_checkequal(x1^1,x1);
assert_checkequal(x1^(-1),inv(x1));
assert_checkequal(x1^2,x1*x1);
assert_checkequal(x1^(-2),inv(x1)^2);

x1(1,1)=x1(1,1)+%i;
assert_checktrue(x1^2==x1*x1);
//if or(x1^(-2)<>inv(x1)^2) then pause,end  //simp complexe non implemented
assert_checktrue(x1^1==x1);
//if or(x1^(-1)<>inv(x1)) then pause,end //simp complexe non implemented


//simple ops
assert_checkfalse(1==0);
assert_checkfalse(1<0);
assert_checkfalse(1<=0);
assert_checktrue(1==1);
assert_checkfalse(~(1==1));
assert_checkfalse(0>1);
assert_checkfalse(0>=1);
assert_checkfalse(1<>1);
assert_checktrue(0<>1);
assert_checkfalse("1"=="0");
assert_checkfalse("1"<>"1");

assert_checkfalse(%s==0);
assert_checkfalse(0==%s);
assert_checktrue(%s==%s);
assert_checkfalse(%s<>%s);
assert_checkfalse(%s==%s+1);
assert_checkfalse(%s+1==%s);

assert_checkfalse(1/%s==0);
assert_checkfalse(0==1/%s);
assert_checktrue(1/%s==1/%s);
assert_checkfalse(1/%s<>1/%s);
assert_checkfalse(1/%s==1/%s+1);
assert_checkfalse(1/%s+1==1/%s);
assert_checktrue(1/%s<>1/%s+1);
assert_checktrue(1/%s+1<>1/%s);

l=list(1,[1 23],"adssa");
l1=list(123,"sdwqqwq");
assert_checkfalse(l==0);
assert_checkfalse(0==l);
assert_checktrue(l==l);
assert_checkfalse(l<>l);
assert_checkfalse(l==l1);
assert_checkfalse(l1==l);
assert_checktrue(l<>l1);
assert_checktrue(l1<>l);

assert_checkfalse(%t&1==2);
assert_checktrue(%t|1==2);
assert_checkfalse(%t&-1==2);
assert_checktrue(%t|-1==2);
assert_checkfalse(1<2&1==2);
assert_checktrue(1<2|1==2);
assert_checkfalse(1<2&-1==2);
assert_checktrue(1<2|-1==2);
assert_checkfalse(2>1&1==2);
assert_checktrue(2>1|1==2);
assert_checkfalse(2>1&-1==2);
assert_checktrue(2>1|-1==2);
assert_checkfalse(1==1&1==2);
assert_checktrue(1==1|1==2);
assert_checkfalse(1==1&-1==2);
assert_checktrue(1==1|-1==2);
assert_checkfalse(1<>2&1==2);
assert_checktrue(1<>2|1==2);
assert_checkfalse(1<>2&-1==2);
assert_checktrue(1<>2|-1==2);

assert_checkfalse(%t&1>2);
assert_checktrue(%t|1>2);
assert_checkfalse(%t&-1>2);
assert_checktrue(%t|-1>2);
assert_checkfalse(1<2&1>2);
assert_checktrue(1<2|1>2);
assert_checkfalse(1<2&-1>2);
assert_checktrue(1<2|-1>2);
assert_checkfalse(2>1&1>2);
assert_checktrue(2>1|1>2);
assert_checkfalse(2>1&-1>2);
assert_checktrue(2>1|-1>2);
assert_checkfalse(1==1&1>2);
assert_checktrue(1==1|1>2);
assert_checkfalse(1==1&-1>2);
assert_checktrue(1==1|-1>2);
assert_checkfalse(1<>2&1>2);
assert_checktrue(1<>2|1>2);
assert_checkfalse(1<>2&-1>2);
assert_checktrue(1<>2|-1>2);
