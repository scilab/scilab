// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function [x1,crit]=karmarkar(a,b,c,x0,eps,Gamma)
maxiter=200;
epsdef=1.d-5
//
[lhs,rhs]=argn(0)
select rhs
case 4 then
  Gamma=1/4,eps=epsdef
case 5 then
  Gamma=1/4
case 6,
else
 error('[x1,crit]=karmarkar(a,b,c,x0 [,eps [,Gamma]])')
end
//verification des donnees
[n,p]=size(a)
w=size(b)
if w(1)<>n then 
  error(msprintf(gettext("%s: Wrong size for input argument #%d."),'karmarkar',2));
end
w=size(c)
if w(1)<>p then
  error(msprintf(gettext("%s: Wrong size for input argument #%d."),'karmarkar',1));
end
w=size(x0)
if w(1)<>p then
  error(msprintf(gettext("%s: Wrong size for input argument #%d."),'karmarkar',4));
end
if min(x0)<0|norm(a*x0-b)>eps then 
  error(msprintf(gettext("%s: x0 is not feasible."),'karmarkar'));
end
//
x1=x0;tc=c';
crit=tc*x1;
test=eps+1
count=0
while test>eps&count<=maxiter
    count=count+1;
//    ax=a*diag(x1);
    ax=a.*(ones(size(a,1),1)*x1');
    xc=x1.*c;
    y=ax'\xc;
//    y=(ax*ax')\(ax*xc)
    d=-xc+ax'*y;
    dk=x1.*d;
    if min(dk)>0 then 
      error(msprintf(gettext("%s: Unbounded problem!"),'karmarkar'));
    end
    alpha=-Gamma/min(d);
    test=alpha*(norm(d)**2)/max(1,abs(crit));
    x1=x1+alpha*dk;
    crit=tc*x1;
    write(%io(2),[count,crit,test],'(f3.0,3x,e10.3,3x,e10.3)')
end
  
endfunction
