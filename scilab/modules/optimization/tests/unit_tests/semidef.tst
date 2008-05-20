// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

F0=[2,1,0,0;
    1,2,0,0;
    0,0,3,1
    0,0,1,3];
F1=[1,2,0,0;
    2,1,0,0;
    0,0,1,3;
    0,0,3,1]
F2=[2,2,0,0;
    2,2,0,0;
    0,0,3,4;
    0,0,4,4];
blck_szs=[2,2];
F01=F0(1:2,1:2);F02=F0(3:4,3:4);
F11=F1(1:2,1:2);F12=F1(3:4,3:4);
F21=F2(1:2,1:2);F22=F2(3:4,3:4);
x0=[0;0];
Z0=2*F0;
Z01=Z0(1:2,1:2);Z02=Z0(3:4,3:4);
FF=[[F01(:);F02(:)],[F11(:);F12(:)],[F21(:);F22(:)]];
ZZ0=[[Z01(:);Z02(:)]];
c=[trace(F1*Z0);trace(F2*Z0)];
options=[10,1.d-10,1.d-10,0,50];
[x,Z,ul,info]=semidef(x0,pack(ZZ0),pack(FF),blck_szs,c,options);
w=vec2list(unpack(Z,blck_szs),[blck_szs;blck_szs]);Z=sysdiag(w(1),w(2));
if c'*x+trace(F0*Z) < %eps then pause,end
spec(F0+F1*x(1)+F2*x(2));
if trace(F1*Z)-c(1) < %eps then pause,end
if trace(F2*Z)-c(2) < %eps then pause,end
 