// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d=%spb_diag(a,k)
// g_diag - implement diag function for sparse matrix, rational matrix ,..

[lhs,rhs]=argn(0)
if rhs==1 then k=0,end
[ij,v,sz]=spget(a)
m=sz(1);n=sz(2)
if m>1&n>1 then
  l=find(ij(:,1)==(ij(:,2)-k))
  if k<=0 then
    mn=mini(m,n-k)
  else
    mn=min(m+k,n)
  end
  kk=abs(k)
  d=sparse([ij(l,1),ones(ij(l,1))],v(l),[mn,1])
else
  nn = max(m,n)+abs(k)
  if ij==[] then 
    d=sparse([],[],[nn,nn])
  else
    d=sparse([ij(:,1),ij(:,1)+k],v,[nn,nn])
  end
end
endfunction
