// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [Slmin]=minss(Sl,tol)

[lhs,rhs]=argn(0)
flag=Sl(1);
 if flag(1)<>'lss' then error(91,1),end
 select rhs
   case 1 then tol=[]
   case 2 then tol=tol
   else error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"minss",1,2))
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
