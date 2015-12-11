//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


format("v",10)
//display of numbers
x=2.6533333
x=[2.6533333 -0.0002]
x'
x=[]

//display of polynomials
z=poly(0,"z")
num=[ (((((1)*z-2.6533333)*z+2.6887936)*z-1.2916784)*z+0.2911572)* ...
z-0.0243497
(((((1)*z-2.6533333)*z+2.6887936)*z-1.2916784)*z+0.2911572)* ...
z-0.0243497
(((1)*z )*z )*z+1
0]
den = [ ((((1)*z-1.536926)*z+0.8067352)*z-0.1682810)*z+0.0113508
((((1)*z-1.536926)*z+0.8067352)*z-0.1682810)*z+0.0113508
((1)*z )*z
1]
num',den'
[num;den]
[num den]
//display of polynomials
z=poly(0,"z")
n=1+z;d=n*n;n1=[n d];d1=[d d*d];n1=[n1 n1];d1=[d1 d1];
n2=[n n*n;n*n n**4];d2=[n*n n**4;n n*n];den=d2(1,:);num=n2(2,:);
p=poly(rand(5,1),"z");q=poly(rand(6,1),"z");
p1=[p,0.1+2.35*z-5.05*z**3];q1=[q,2.56+0.03*z-10.01*z*z+z**5];
d3=[1+z**10 z**15];
//display of rationals
r=num./den
r'
rlist(d,n)
rlist(n,d)
rlist(d2,n2)
rlist(n2,d2)
rlist(den,num)
rlist(num,den)
rlist(p1,q1)
rlist(q1,p1)
rlist(p,q)
rlist(q,p)
rlist(p,z)
rlist(z,p)
rlist(d1,n1)
rlist(n1,d1)
//display of strings
digits="abcdefghijklmnopqrstuvwxyz"
numbers="1234567890"
majuscules="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
symbols=",./;''[] \ =-!\$%^&*()_+~:""[]| @"
[numbers;digits]
[numbers digits;digits numbers]
[numbers digits+digits+digits]
ans';
[ans ans]
""

oldlines = lines();
lines(0,50);
digits="abcdefghijklmnopqrstuvwxyz";
d = digits+digits+digits;
n = "1234567890";

// scalar
n
n+n+n+n+n
d
d+d+d+d

// column vector
[n; n; n; n; n]
[d; d; d; d; d]
[n+n; n+n; n+n]
[n+n; n+n+n; n+n]
[n+n; d+d; n+n]
[d; d]
[d; n]
[n; d]
[n; n]

// row vector
[n n n n n]
[d d d d d]
[n+n n+n n+n]
[n+n n+n+n n+n]
[d d]
[d n]
[n d]
[n n]

// matrix
nd = [n d];
dn = [d n];

[nd nd]
[nd; nd]
[nd' nd']
[nd'; nd']

[dn dn]
[dn; dn]
[dn' dn']
[dn'; dn']

lines(oldlines);

// display of list
list()
list(1,2,3)
list(list(1,list()),2)
list(rlist(q,p),1.23)
// overloading of display
1:$
list(1,1:$)
list(list(1:$))
list(list(1,$),list())
// Tlist overloaded display
TL=tlist("x",1,2)
deff("%x_p(x)","disp(x(3),x(2),''Mydisplay'')")
TL
list(TL)
list(list(TL))
TL=tlist("x",1:$,2)
list(TL)
list(list(TL))
TL(2)=TL

// boolean
%t
%f
b=[%t %t %t]
~b
bp=b'
~bp
bb=[b; b; b]
~bb
