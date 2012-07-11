// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8174 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8174
//
// <-- Short Description -->
// ss2tf produces bad results if applied to a complex state space system
//A real dynamical system represented in its eigen state space basis
//(A,B,C) matrices are complex but the system is real
A = [ 0.75+%i*0.6,0;0, 0.75-%i*0.6];
B = [ 0.2-%i*0.45; 0.2+%i*0.45];
C = [-0.04+%i*0.1,-0.04-%i*0.1];
Sys =syslin('d',A,B,C,0);
H = ss2tf(Sys);
if norm(coeff((H.num-(-0.1011+0.074*%z))))>10*%eps then pause,end
if norm(coeff((H.den-(0.9225-1.5*%z+%z^2))))>10*%eps then pause,end
