//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - INRIA - Serge Steer
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//Vector of complex numbers

R=[2 1+%i  2-3*%i];
[wn,zeta]=damp(R);
assert_checkequal (wn,abs(R));
assert_checkequal (zeta,-real(R)./wn);

dt=1;
[wn,zeta]=damp(R,dt);
Rd=log(R)/dt;
assert_checkequal (wn,abs(Rd));
assert_checkequal (zeta,-real(Rd)./wn);

R=0;
[wn,zeta]=damp(R);
assert_checkequal (wn,0);
assert_checkequal (zeta,-1);

R=0;dt=1;
[wn,zeta]=damp(R,dt);
assert_checkequal (wn,%inf);
assert_checkequal (zeta,%nan);

R=1;dt=1;
[wn,zeta]=damp(R,dt);
assert_checkequal (wn,0);
assert_checkequal (zeta,-1);

R=1;
[wn,zeta]=damp(R);
assert_checkequal (wn,1);
assert_checkequal (zeta,-1);

//Polynomial and Polynomial array
P=real([poly([2 1+%i 1-%i  2-3*%i 2+3*%i],'s'),poly(0,'s'),poly(1,'s')]);
[wn,zeta]=damp(P);
wnref=[0;1;sqrt(2);sqrt(2);2;sqrt(13);sqrt(13)];
zetaref=[-1;-1;-sqrt(2)/2;-sqrt(2)/2;-1;-2/sqrt(13);-2/sqrt(13)];
assert_checkalmostequal (wn,wnref);
assert_checkalmostequal (zeta,zetaref);

[wn,zeta]=damp(prod(P));
wnref=[0;1;sqrt(2);sqrt(2);2;sqrt(13);sqrt(13)];
zetaref=[-1;-1;-sqrt(2)/2;-sqrt(2)/2;-1;-2/sqrt(13);-2/sqrt(13)];
assert_checkalmostequal (wn,wnref);
assert_checkalmostequal (zeta,zetaref);

dt=1;
[wn,zeta]=damp(P,dt);
t1=log(1+%i)/dt;
t2=log(2-3*%i)/dt;
wnref=[0;log(2);abs(t1);abs(t1);abs(t2);abs(t2);%inf];
zetaref=[-1;-1;-real(t1)/abs(t1);-real(t1)/abs(t1);-real(t2)/abs(t2);-real(t2)/abs(t2);%nan];
assert_checkalmostequal (wn,wnref);
assert_checkalmostequal (zeta(2:$),zetaref(2:$));
assert_checktrue(abs(abs(zeta(1))-1)<100*%eps);

dt=1;
[wn,zeta]=damp(prod(P),dt);
t1=log(1+%i)/dt;
t2=log(2-3*%i)/dt;
wnref=[0;log(2);abs(t1);abs(t1);abs(t2);abs(t2);%inf];
zetaref=[1;-1;-real(t1)/abs(t1);-real(t1)/abs(t1);-real(t2)/abs(t2);-real(t2)/abs(t2);%nan];
assert_checkalmostequal (wn,wnref, 0, 1D-14);
assert_checkalmostequal (zeta(2:$),zetaref(2:$));
assert_checktrue(abs(abs(zeta(1))-1)<100*%eps);

//transfer function
[wn,zeta]=damp(syslin('c',ones(1,3),P));
wnref=[0;1;sqrt(2);sqrt(2);2;sqrt(13);sqrt(13)];
zetaref=[-1;-1;-sqrt(2)/2;-sqrt(2)/2;-1;-2/sqrt(13);-2/sqrt(13)];
assert_checkalmostequal (wn,wnref, 0, 1D-14);
assert_checkalmostequal (zeta(2:$),zetaref(2:$));
assert_checktrue(abs(abs(zeta(1))-1)<100*%eps);

dt=1;
[wn,zeta]=damp(syslin(dt,ones(1,3),P));
t1=log(1+%i)/dt;
t2=log(2-3*%i)/dt;
wnref=[0;log(2);abs(t1);abs(t1);abs(t2);abs(t2);%inf];
zetaref=[1;-1;-real(t1)/abs(t1);-real(t1)/abs(t1);-real(t2)/abs(t2);-real(t2)/abs(t2);%nan];
assert_checkalmostequal (wn,wnref,0, 1D-14);
assert_checkalmostequal (zeta(2:$),zetaref(2:$));
assert_checktrue(abs(abs(zeta(1))-1)<100*%eps);

//state-space
[wn,zeta]=damp(tf2ss(syslin('c',1,prod(P))));
wnref=[0;1;sqrt(2);sqrt(2);2;sqrt(13);sqrt(13)];
zetaref=[-1;-1;-sqrt(2)/2;-sqrt(2)/2;-1;-2/sqrt(13);-2/sqrt(13)];
assert_checkalmostequal (wn,wnref);
assert_checkalmostequal (zeta(2:$),zetaref(2:$));
assert_checktrue(abs(abs(zeta(1))-1)<100*%eps);

dt=1;
[wn,zeta]=damp(tf2ss(syslin(dt,1,prod(P))));
t1=log(1+%i)/dt;
t2=log(2-3*%i)/dt;
wnref=[0;log(2);abs(t1);abs(t1);abs(t2);abs(t2);%inf];
zetaref=[1;-1;-real(t1)/abs(t1);-real(t1)/abs(t1);-real(t2)/abs(t2);-real(t2)/abs(t2);%nan];
assert_checkalmostequal (wn, wnref, 0, 1D-14) ;
assert_checkalmostequal (zeta(2:$),zetaref(2:$)) ;
assert_checktrue(abs(abs(zeta(1))-1)<100*%eps);

//invalid calls
assert_checkfalse(execstr("[wn,zeta]=damp()","errcatch")==0);
assert_checkfalse(execstr("[wn,zeta]=damp(%t)","errcatch")==0);
assert_checkfalse(execstr("[wn,zeta]=damp(P,1:3)","errcatch")==0);
assert_checkfalse(execstr("[wn,zeta]=damp(P,%t)","errcatch")==0);
