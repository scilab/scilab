// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// English imposed for warnings
// <-- ENGLISH IMPOSED -->
//
// Ref not checked because of many lsode outputs which are different according to platform (x86/x64).
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 11980 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11980
//
// <-- Short Description -->
// Submatrix incorrectly defined.
// at line     148 of function csim called by :  
// y=csim('step',t,numerador/denominador).

s=poly(0,'s'); funcprot(0);
kp=96.770532;
ki=42.568724;
kd=18.711118;
numerador=s^5*kd+s^4*(60*kd+kp)+s^3*(1500*kd-60*kp+ki)+s^2*(-15000*kd+1500*kp-60*ki)+s*(-15000*kp+1500*ki)-15000*ki;
denominador=s^5*(kd)+s^4*(-1-60*kd+kp)+s^3*(-1500-60+1500*kd-60*kp+ki)+s^2*(-15000-1500-15000*kd+1500*kp-60*ki)+s*(-15000-15000*kp+1500*ki)-15000*ki;
t=0:0.01:100;
errMsg = msprintf(_("%s: Simulation failed before final time is reached.\n"),"csim");
assert_checkerror("csim(""step"",t,numerador/denominador);", errMsg);
