//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1595 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1595
//
// <-- Short Description -->
//    When using the ode solver with a computation at mesh points (itask =2) in a
//    backward integration the result is given at only one point.

 function xdot = RHS(t,x),xdot=x,endfunction;
%ODEOPTIONS = [2,0,0,%inf,0,2,500,12,5,0,-1,-1];
sol = ode('adams',0,0,-10,RHS);
if size(sol,2)<>3 then pause,end
