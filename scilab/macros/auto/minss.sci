function [Slmin]=minss(Sl,tol)
// Copyright INRIA
[lhs,rhs]=argn(0)
flag=Sl(1);
 if flag(1)<>'lss' then error(91,1),end
 select rhs
   case 1 then tol=[]
   case 2 then tol=tol
   else error('1 or 2 inputs: sl [,tol]')
 end;
 [a,b,c,d,x0,dom]=Sl(2:7)
//
if tol<>[] then
   [nc,u1]=contr(a',c',tol)
 else
   if [a;c]==[] then reltol=0;else reltol=1.d-10*norm([a;c],1);end
   [nc,u1]=contr(a',c',reltol)
end
u=u1(:,1:nc)
c=c*u;a=u'*a*u;b=u'*b,x0=u'*x0;
if tol<>[] then
  [no,u2]=contr(a,b,tol)
else
  if [a,b]==[] then reltol=0;else reltol=1.d-10*norm([a,b],1);end
  [no,u2]=contr(a,b,reltol)
end
u=u2(:,1:no)
a=u'*a*u;b=u'*b;c=c*u
if lhs==1 then Slmin=syslin(dom,a,b,c,d,u'*x0),end
//Would be nice to return U=U1*U2
endfunction
