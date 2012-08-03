// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3796 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3796
//
// <-- Short Description -->

//In some situation the "tf2ss()" function is not capable to compute
//correctly the state space representation of a transfer function.

s = poly(0,'s');

E  = 15.0 ;
Vo =  5.0 ;

D = Vo/E      ;
Delta_Vo = 0.1;
Ts = 1*10^(-3);
t_on = D*Ts ;
t_of = Ts-t_on;
Ro = 5.1 ;
L  = 2.28*10^(-3);

Error=0.1;    // ramp error desired
Delta_i_L=(E*(1-D)*D*Ts)/L;
C = (Delta_i_L*Ts)/(8*Delta_Vo);
fdt_Vo_d = syslin('c',E,((L*C)*s^(2)+(L/Ro)*s+1) );
fa0 = 50;        // [Hertz]
wa0 = 2*%pi*fa0; // [rad/sec] pulsation

Kc=1/(E*Error);
Ctipo=syslin('c',Kc,s);

K=10^(35/20);
ma=20;
wta=0.8;
Ca=syslin('c',1+(wta/wa0)*s,1+s*(wta/wa0)/ma);
Cds = K*Ctipo*Ca*Ca;
Lds = Cds*fdt_Vo_d;
uno = syslin('c',1,1);
Wds =Lds/.uno;

Wds_ss = tf2ss(Wds);
if size(Wds_ss.A)<>5 then pause,end
