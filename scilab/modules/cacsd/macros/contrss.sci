function slc=contrss(a,tol)
// Copyright INRIA
[lhs,rhs]=argn(0)
//
flag=a(1);
 if flag(1)<>'lss' then
     error(91,1),
 end;
if rhs==1 then tol=sqrt(%eps);end
if rhs>2 then error('1 or 2 inputs to contrss : sl [,tol]')
 end;
 [a,b,c,d,x0,dom]=a(2:7)
//
[nc,u]=contr(a,b,tol*norm([a,b],1))
u=u(:,1:nc)
a=u'*a*u;b=u'*b;c=c*u
slc=syslin(dom,a,b,c,d,u'*x0)
endfunction
