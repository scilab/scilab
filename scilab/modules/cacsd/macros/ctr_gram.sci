// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [gc]=ctr_gram(a,b,domaine)
//!

[lhs,rhs]=argn(0)
select type(a)
case 1  then
  if rhs<2 then error(39); end;
  if rhs==2 then domaine='c'; end;
  if part(domaine,1)<>'c' then domaine='d',end
  [m,n]=size(a)
  if m<>n then error(20,1),end
  [mb,nb]=size(b);if mb<>n then error(60),end
case 16 then
  flag=a(1);
  if flag(1)=='r' then a=tf2ss(a);end
  if flag(1)<>'lss' then error(91,1),end
  [a,b,domaine]=a([2,3,7])
  if domaine==[] then
    write(%io(2),'Warning: ctr_gram --> By default time-domain is continuous')
    domaine='c';
  end
  [n,n]=size(a)    
else 
  error('(a,b) pair or syslin state-space')
end
s=spec(a)
if part(domaine,1)=='c' then
  if maxi(real(s))>=0 then error('Unstable system'),end
else
  if maxi(abs(s))>=1 then error('Unstable system'),end
end
gc=lyap(a',-b*b',domaine)
endfunction
