// Non-regression test file for bug 631
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 10 janvier 2006

mode(-1);
clear
p=-[1;1];C=[1 -1];b=zeros(1,1);ci=zeros(2,1);cs=[1;0];me=1;x0=zeros(2,1)
[x,lagr,f]=linpro(p,C,b,ci,cs,me ,x0)
ok=%t
if abs(C*x)>%eps then ok=%f,end
if or(~(ci<=x&x<=cs)) then ok=%f,end
affich_result(ok,631);
