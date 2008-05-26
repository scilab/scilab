// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [go]=obs_gram(a,c,domaine)

[lhs,rhs]=argn(0)
select type(a)
case 1  then
  if rhs<2 then error(39); end;
  if rhs==2 then domaine='c'; end;
  if part(domaine,1)<>'c' then domaine='d',end
  [m,n]=size(a)
  if m<>n then error(20,1),end
  [mb,nb]=size(c);if nb<>n then error(60),end
case 16 then
  flag=a(1)
  if flag(1)=='r' then a=tf2ss(a),end
  if flag(1)<>'lss' then error(91,1),end
  [a,c,domaine]=a([2,4,7])
  if domaine==[] then
    write(%io(2),msprintf(gettext("%s: By default time-domain = continuous.\n"),"obs_gram"))
    domaine='c';
  end
  [n,n]=size(a)
else 
  error(msprintf(gettext("%s: (A,C) pair or syslin list.\n"),"obs_gram"))
end;
//
s=spec(a)
if part(domaine,1)=='c' then
  if maxi(real(s))>=0 then error(msprintf(gettext("%s: Unstable system.\n"),"obs_gram")),end
else
  if maxi(abs(s))>=1 then error(msprintf(gettext("%s: Unstable system.\n"),"obs_gram")),end
end
go=lyap(a,-c'*c,domaine)
endfunction
