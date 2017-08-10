//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
ny=1,nu=1,sig=0.01;
ar=armac(1,[0,0.2],[0,1],ny,nu,sig);
[S,Sn]=arma2ss(ar);
A=[0,0.2;0,0];B=[0;1];C=[1,0];D=0;
assert_checkequal(S.A,A);
assert_checkequal(S.B,B);
assert_checkequal(S.C,C);
assert_checkequal(S.D,D);
A=[0,1;0,0];B=[0;0.01];C=[1,0];D=0;
assert_checkequal(Sn.A,A);
assert_checkequal(Sn.B,B);
assert_checkequal(Sn.C,C);
assert_checkequal(Sn.D,D);
//
ny=1,nu=1;
ar=armac([1,-0.8,0.2],0.001,0,ny,nu,0);
[S,Sn]=arma2ss(ar);
A=[0.8,-0.2;1,0];B=[0.001;0];C=[1,0];D=0;
assert_checkequal(S.A,A);
assert_checkequal(S.B,B);
assert_checkequal(S.C,C);
assert_checkequal(S.D,D);
A=[0.8,-0.2;1,0];B=[0;0];C=[1,0];D=0;
assert_checkequal(Sn.A,A);
assert_checkequal(Sn.B,B);
assert_checkequal(Sn.C,C);
assert_checkequal(Sn.D,D);



a=[1,-2.851,2.717,-0.865].*.eye(2,2);
b=[0,1,1,1].*.[1;1];
d=[1,0.7,0.2].*.eye(2,2);
sig=eye(2,2);
ar=armac(a,b,d,2,1,sig);

[S,Sn]=arma2ss(ar);

A=[2.851,0,    -2.717,0,     0.865,0,    1,1,1;
   0,    2.851,0,     -2.717,0,    0.865,1,1,1;
   1,    0,    0,      0,    0,    0,    0,0,0;
   0,    1,    0,      0,    0,    0,    0,0,0;
   0,    0,    1,      0,    0,    0,    0,0,0;
   0,    0,    0,      1,    0,    0,    0,0,0;
   0,    0,    0,      0,    0,    0,    0,0,0;
   0,    0,    0,      0,    0,    0,    1,0,0;
   0,    0,    0,      0,    0,    0,    0,1,0];
B=[0;0;0;0;0;0;1;0;0];
C=[1,0,0,0,0,0,0,0,0;
   0,1,0,0,0,0,0,0,0];
D=[0;0];
assert_checkequal(S.A,A);
assert_checkequal(S.B,B);
assert_checkequal(S.C,C);
assert_checkequal(S.D,D);

A=[2.851,0,-2.717,0,0.865,0,0.7,0,0.2,0;
   0,2.851,0,-2.717,0,0.865,0,0.7,0,0.2;
   1,0,0,0,0,0,0,0,0,0;
   0,1,0,0,0,0,0,0,0,0;
   0,0,1,0,0,0,0,0,0,0;
   0,0,0,1,0,0,0,0,0,0;
   0,0,0,0,0,0,0,0,0,0;
   0,0,0,0,0,0,0,0,0,0;
   0,0,0,0,0,0,1,0,0,0;
   0,0,0,0,0,0,0,1,0,0];
B=[1,0;0,1;0,0;0,0;0,0;0,0;1,0;0,1;0,0;0,0];
C=[1,0,0,0,0,0,0,0,0,0;0,1,0,0,0,0,0,0,0,0];
D=[0,0;0,0];
assert_checkequal(Sn.A,A);
assert_checkequal(Sn.B,B);
assert_checkequal(Sn.C,C);
assert_checkequal(Sn.D,D);
