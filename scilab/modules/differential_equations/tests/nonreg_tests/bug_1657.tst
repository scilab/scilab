// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1657 -->
// NOT FIXED
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1657
//
// <-- Short Description -->
// 'ode' is not reenterable
// !--error 22 
// recursion problems. Sorry....
// at line       2 of function Alpha called by :  
// Y= ode(1,0,2,alpha);

// call ode within the rhs-function to a different ode call

// <-- NOT FIXED -->

function fz=alpha_rhs(t,z)
  fz= -z; // exp(-t)
endfunction

function fy=Alpha(t,y)
  y0=1.
  t0=0.
  fy= 2*ode(y0,t0,t,alpha_rhs) // should be 2*exp(-t)
endfunction
// The solution is -2*exp(-t)+3
y0=1.
t0=0.
t1=2.
Ycomputed= ode(y0,t0,t1,Alpha);
Yexpected=-2*exp(-2)+3 // Approximately   2.729329433526774
if (abs(Ycomputed-Yexpected)>%eps) then pause,end

