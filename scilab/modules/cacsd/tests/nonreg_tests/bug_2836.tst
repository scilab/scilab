//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA -Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3836 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3836
//
// <-- Short Description -->
//Problem with the function csim when using state space form. When using this
//function with two output arguments [y, x] (output and state of the dynamical
//system), with an observation matrix C=[1 0], the output is not equal to the first
//component of the state.

Ks = 2; z = 0.9; w = 10;
A=[0 1 ; -w^2 -2*z*w];
B=[0 ; Ks*w^2];
C=[1 0];
sys=syslin('c',A,B,C);
u=[2*ones(1,280) 0.5*ones(1,240) 1.5*ones(1,380) ones(1,160)];
N=length(u);
t=linspace(0,2,N);
[y,x] = csim(u,t,sys);
if norm(y-x(1,:))>%eps then pause,end

