// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 631 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=631
//
// <-- Short Description -->
//    linpro with initial guess does not work correctly.
//    linpro(-[1;1], [1 -1], zeros(1,1), zeros(2,1), [1;0], 1, 
//    zeros(2,1)) returns:
//     ans  =
//
//    !   1. !
//    !   1. !,
//    which violates the upper-bound constraint ([1;0]).
//
//    On the other hand, linpro without initial guess works fine.
//    -->linpro(-[1;1], [1 -1], zeros(1,1), zeros(2,1), [1;0], 1)
//     ans  =
//
//    !   0. !
//    !   0. !

p  = -[1;1];
C  = [1 -1];
b  = zeros(1,1);
ci = zeros(2,1);
cs = [1;0];
me = 1;
x0 = zeros(2,1)

[x,lagr,f] = linpro(p,C,b,ci,cs,me ,x0);

if abs(C*x)>%eps      then pause,end
if or(~(ci<=x&x<=cs)) then pause,end
