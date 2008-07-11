// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA -Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 68 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=68
//
// <-- Short Description -->
//Precision problem with the trzeros function, 

s=poly(0,'s'); 
A=[-113.63636,-2840909.1,113.63636,2840909.1,0,0;
   1,0,0,0,0,0;
   347.22222,8680555.6,-366.66667,-11111111,19.444444,2430555.6;
   0,0,1,0,0,0;
   0,0,50,6250000,-50,-6250000;
   0,0,0,0,1,0];

System =syslin('c',A,[1;0;0;0;0;0],[0 0 0 1 0 0]);

Td=1/0.1;alpha=1000;Ti=1/0.1;Tr=1/10000;Kp=1e2;
PID=tf2ss(syslin('c',Kp*(1+Td*s)/(1+Td/alpha*s)));

Hrond1=PID*System;closed1=(1/.(Hrond1));
ClosedZeros1=trzeros(closed1);

Hrond2=System*PID;closed2=(1/.(Hrond2));
ClosedZeros2=trzeros(closed2);

C1=gsort([real(ClosedZeros1) imag(ClosedZeros1)],'lr');
C2=gsort([real(ClosedZeros2) imag(ClosedZeros2)],'lr');

if norm(C1-C2)>1d-5 then pause,end

