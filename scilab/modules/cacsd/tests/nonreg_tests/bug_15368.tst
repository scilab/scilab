// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Lucien POVY
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15368 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15368
//
// <-- Short Description -->
// For some continuous time systems, freson() wrongly returned []

s = %s;
sl1 = syslin("c",1,s*(s+1)*(1+s/3))
[K,P] = kpure(sl1)
sl = K*sl1
taud = 1/imag(P)
sltaud=sl*(1+taud*s)//first open loop
sltaud2=sl*(1+2*taud*s)//second open loop
ff = freson(sltaud/(1+sltaud))//freson of first close loop
assert_checkalmostequal(ff, 0.32447218051387, 1e-14);

SLBF = sltaud2/(1+sltaud2); //THE CLOSE LOOP OF sltau2
fr6 = freson(SLBF);
assert_checkalmostequal(fr6, 0.48257117694410, 1e-14);

