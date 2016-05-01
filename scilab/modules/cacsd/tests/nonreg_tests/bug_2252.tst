//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2252 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2252
//
// <-- Short Description -->
//   csim produces unusual results for the following nonminimum phase system
//   Problem due to state basis change used to block diagonalize the state matrix
s=poly(0,'s');
p=(1-s)/(1+s)^2;
p=syslin('c',p);
t=0:0.01:20;
ycsim=csim('step',t,p);

sl=syslin('c',[-1 -2;0 -1],[1;1],[-1 0]);
function ydot=sim(t,y),ydot=sl.a*y+sl.b,endfunction
yode=sl.c*ode(zeros(2,1),0,t,1.414D-09,0.0000001,sim);

assert_checkalmostequal(ycsim, yode, [], 7e-3, "matrix");
