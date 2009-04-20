// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

 // to check that ode works
 // ---------- Simple one dimension ODE (Scilab function external)
 // dy/dt=y^2-y sin(t)+cos(t), y(0)=0
 function ydot=f(t,y),ydot=y^2-y*sin(t)+cos(t),endfunction
 y0=0;t0=0;t=0:0.1:%pi;
 y=ode(y0,t0,t,f);
 if size(y) <> [1 32] then pause,end