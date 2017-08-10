//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
s=poly(0,'s');
P22=syslin("c",1/(s+2));
[P,r]=lqg2stan(P22,eye(2,2),eye(2,2));
assert_checktrue(and(P.num==[1,0,1;0,0,1;1,1,1]));
assert_checktrue(and(P.den==[2+s,1,2+s;1,1,1;2+s,1,2+s]));
assert_checktrue(and(r==[1 1]));

P22=syslin("c",[1;1]/(s+2));
[P,r]=lqg2stan(P22,eye(2,2),eye(3,3));

Nref=[1,0,0,sqrt(2);0,0,0,1;sqrt(2)/2,1,0,1;sqrt(2)/2,0,1,1];
Dref=[2,1,1,2,1,0,0,1;1,1,1,1,0,0,0,0;2,1,1,2,1,0,0,1;2,1,1,2,1,0,0,1];
assert_checkalmostequal(coeff(P.num),Nref,1e-10,1e-10);
assert_checkalmostequal(coeff(P.den),Dref,1e-10,1e-10);
assert_checktrue(and(r==[2 1]));

P22=syslin("c",[1 1]/s);
[P,r]=lqg2stan(P22,eye(3,3),eye(2,2));
Nref=[1,0,-1,-1;0,0,1,0;0,0,0,1;-1,1,1,1];
Dref=[0,1,0,0,1,0,1,1;1,1,1,1,0,0,0,0;1,1,1,1,0,0,0,0;0,1,0,0,1,0,1,1];
assert_checkalmostequal(coeff(P.num),Nref,1e-10,1e-10);
assert_checkalmostequal(coeff(P.den),Dref,1e-10,1e-10);
assert_checktrue(and(r==[1 2]));

A=[1 2;-1 0];B=[1;0];C=[0 1];
P22=syslin("c",A,B,C);
[P,r]=lqg2stan(P22,eye(3,3),eye(3,3));
assert_checkequal(P.A,P22.A);
assert_checkequal([1,0,0,1;0,1,0,0],P.B);
assert_checkequal([1,0;0,1;0,0;0,1],P.C);
assert_checkequal([0,0,0,0;0,0,0,0;0,0,0,1;0,0,1,0],P.D);
assert_checkequal(P22,P($,$));
assert_checktrue(and(r==[1 1]));

A=[1 2;-1 0];B=[1 0;1 1];C=[0 1];
P22=syslin("c",A,B,C);
[P,r]=lqg2stan(P22,eye(4,4),eye(3,3));
assert_checkequal(P.A,P22.A);
assert_checkequal([1,0,0,1,0;0,1,0,1,1],P.B);
assert_checkequal([1,0;0,1;0,0;0,0;0,1],P.C);
assert_checkequal([0,0,0,0,0;0,0,0,0,0;0,0,0,1,0;0,0,0,0,1;0,0,1,0,0],P.D);
assert_checkequal(P22,P($,$-1:$));
assert_checktrue(and(r==[1 2]));
