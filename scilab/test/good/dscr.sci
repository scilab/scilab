// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [f,r]=dscr(a,dt,m)

  [lhs,rhs]=argn(0);
  if type(dt)<>1 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"),"dscr",2))
  end
  if size(dt,'*')<>1 then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),"dscr",2))
  end
  if ~isreal(dt) then
    error(msprintf(gettext("%s: Input argument #%d must be real.\n"),"dscr",2))
  end

  if dt<=0 then
    error(msprintf(gettext("%s: Input argument #%d must be strictly positive.\n"),"dscr",2))
  end

  select typeof(a)
  case 'rational' then
    a=tf2ss(a);
    [a,b,c,d,x0,dom]=a(2:7);
  case 'state-space' then
    [a,b,c,d,x0,dom]=a(2:7)
  else 
    error(97,1),
  end;
  if dom<>'c' then 
    warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"dscr",1))
  end
  [n1,m1]=size(b),
  s=expm([a,b;0*ones(m1,n1+m1)]*dt),
  f=s(1:n1,1:n1);g=s(1:n1,n1+1:n1+m1);
  if rhs==3 then
    s=expm([-a,m;0*a a']*dt),
    r=f*s(1:n1,n1+1:n1+n1),
  end;
  f=syslin(dt,f,g,c,d,x0)
endfunction
