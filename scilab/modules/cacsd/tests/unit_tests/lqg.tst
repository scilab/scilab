//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
A=[0 1 0;0 0 1;1 0 0];
B=[0.3 1;0 1;-0.3 0.9];
C=[1.9 1.3 1];
D=[0.53 -0.61];
Q_ww=[4 2 0;2 1 0;0 0 1];
R_vv=0.7;
Q_wv=sysdiag(Q_ww,R_vv);
Q_xu=sysdiag(0.1*eye(3,3),[1 0;0 2]);

Qi=1;
// continuous time----------------------------------------
sys=syslin("c",A,B,C,D);

KLQG = lqg(sys,Q_xu,Q_wv);
H=sys/.(-KLQG);
assert_checktrue(and(real(spec(H.A))<0));

[aug,r]=lqg2stan(sys,Q_xu,Q_wv);
KLQG_a = lqg(aug,r);
assert_checktrue(norm(KLQG.A-KLQG_a.A,1)<1e10&norm(KLQG.B-KLQG_a.B,1)<1e10&norm(KLQG.C-KLQG_a.C,1)<1e10);

KLQG1 = lqg(sys,Q_xu,Q_wv,Qi,1);
H1=sys/.(KLQG1);
assert_checktrue(and(real(spec(H1.A))<0));


KLQG2 = lqg(sys,Q_xu,Q_wv,Qi,2);
H2=lft([1;1]*sys*KLQG2,1);
assert_checktrue(and(real(spec(H2.A))<0));
// dicrete time----------------------------------------
sysd=dscr(sys,0.01);

KLQG = lqg(sysd,Q_xu,Q_wv);
H=sys/.(-KLQG);
assert_checktrue(and(abs(spec(H.A))<1));

[aug,r]=lqg2stan(sysd,Q_xu,Q_wv);
KLQG_a = lqg(aug,r);
assert_checktrue(norm(KLQG.A-KLQG_a.A,1)<1e10&norm(KLQG.B-KLQG_a.B,1)<1e10&norm(KLQG.C-KLQG_a.C,1)<1e10);

KLQG1 = lqg(sysd,Q_xu,Q_wv,Qi,1);
H1=sysd/.(KLQG1);
assert_checktrue(and(abs(spec(H1.A))<1));

KLQG2 = lqg(sysd,Q_xu,Q_wv,Qi,2);
H2=lft([1;1]*sysd*KLQG2,1);
assert_checktrue(and(abs(spec(H2.A))<1));
