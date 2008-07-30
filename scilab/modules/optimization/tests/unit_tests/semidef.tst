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
    0,0,3,1];
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
abstol=1.d-8;
reltol=1.d-10;
options=[10,abstol,reltol,0,50];
warning('off');
[x,Z,ul,info]=semidef(x0,pack(ZZ0),pack(FF),blck_szs,c,options);
warning('on');
w=vec2list(unpack(Z,blck_szs),[blck_szs;blck_szs]);
Z=sysdiag(w(1),w(2));
// Check that info states that absolute convergence occured
if info(1)<>2 then pause,end
// Check that ul contains the expected values
expectedUl1=c'*x;
if abs(expectedUl1-ul(1))>100*%eps then pause,end
expectedUl2=-trace(F0*Z);
if abs(expectedUl2-ul(2))>100*%eps then pause,end
// Check Slater's condition, which states that the duality gap is zero at optimum.
dualitygap = ul(1)-ul(2);
if dualitygap > abstol then pause,end


