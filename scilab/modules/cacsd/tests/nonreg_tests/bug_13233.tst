//<-- CLI SHELL MODE -->
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13233 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13233
//
// <-- Short Description -->
// There is an offset in the result of a step response for transfer function
// (a*s^2+b*s+c)/(d*s^2+e*s+f)

s=poly(0,'s'); 
Uo=6;
Da=0.5;
Go=1/15;
L=0.00009;
Rl=0.05;
C=0.0004;
Rc=0.1;
Rt=0.2;
Rd=0.07;
Rz=Rl+Rt*Da+Rd*(1-Da);
Cz=C*(1+Rc*Go);A0=Uo*Rz;
A1=Uo*(L+C*Rc*Rz);
A2=Uo*L*C*Rc;
B0=1+Rz*Go;
B1=L*Go+C*Rc+Rz*Cz;
B2=L*Cz;
H=syslin('c',-(A2*s^2+A1*s+A0)/(B2*s^2+B1*s+B0));
t=[0:0.000001:0.005];
ycsim=csim('step',t,H);

sl=tf2ss(H);
deff("[ydot]=%sim2(t,y)","ydot=sl.a*y+sl.b*1");
x=ode("adams",zeros(2,1),0,t,414d-9,0.000001,%sim2);
yode=sl.c*x+sl.d*1;
assert_checkalmostequal(ycsim,yode,[],1e-4);
