// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//<-- CLI SHELL MODE -->
// <-- Non-regression test for bug 7794 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7794
//
// <-- Short Description -->
//  Example of findABCD help page failed.

A = [ 0.5, 0.1,-0.1, 0.2;
      0.1, 0,  -0.1,-0.1;      
     -0.4,-0.6,-0.7,-0.1;  
      0.8, 0,  -0.6,-0.6];      
B = [0.8;0.1;1;-1];
C = [1 2 -1 0];
SYS=syslin(0.1,A,B,C);
nsmp=100;
U=prbs_a(nsmp,nsmp/5);
Y=(flts(U,SYS)+0.3*rand(1,nsmp,'normal'));

// Compute R
S=15;
[R,N1,SVAL] = findR(S,Y',U');
N=3;
SYS1 = findABCD(S,N,1,R) ;SYS1.dt=0.1;

SYS1.X0 = inistate(SYS1,Y',U');
expectedX0 = [0.180418869; 0.18003251391; -0.06993989023];
assert_checkalmostequal(SYS1.X0, expectedX0, 1.e-8);


