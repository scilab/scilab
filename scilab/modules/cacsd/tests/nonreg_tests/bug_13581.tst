// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 13581 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13581
//
// <-- Short Description -->
// The example of inistate help page did not work on Windows 64bit

//generate data from a given linear system
A = [ 0.5, 0.1,-0.1, 0.2;
0.1, 0,  -0.1,-0.1;
-0.4,-0.6,-0.7,-0.1;
0.8, 0,  -0.6,-0.6];
B = [0.8;0.1;1;-1];
C = [1 2 -1 0];
SYS=syslin(0.1,A,B,C);
nsmp=100;
U=prbs_a(nsmp,nsmp/5);
Y=(flts(U,SYS)+0.3*rand(1,nsmp,"normal"));
// Compute R
S=15;
[R,N1,SVAL] = findR(S,Y',U');
N=3;
SYS1 = findABCD(S,N,1,R) ;
SYS1.dt=0.1;
x0 = inistate(SYS1,Y',U');
expected = [0.180418869191; 0.180032513919; -0.069939890233];
assert_checkalmostequal(x0, expected);
