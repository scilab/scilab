// <-- Non-regression test for bug 631 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=631
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


// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 10 janvier 2006

p=-[1;1];C=[1 -1];b=zeros(1,1);ci=zeros(2,1);cs=[1;0];me=1;x0=zeros(2,1)
[x,lagr,f]=linpro(p,C,b,ci,cs,me ,x0) 
ok=%t
if abs(C*x)>%eps then ok=%f,end
if or(~(ci<=x&x<=cs)) then ok=%f,end
affich_result(ok,631);
