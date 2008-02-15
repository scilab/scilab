// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function d=%b_diag(a,k)

[lhs,rhs]=argn(0)
if rhs==1 then k=0,end
[m,n]=size(a)
if m>1&n>1 then
  if k<=0 then
    mn=mini(m+k,n)
    i0=-k+1
  else
    mn=min(m,n-k)
    i0=k*m+1
  end
  a=matrix(a,m*n,1)
  i=i0+((0:mn-1)*(m+1))
  d=a(i)
else
  nn = max(m,n)+abs(k)
  mn=max(m,n)
  i=(1:mn)+((1:mn)+(k-1))*nn
  d(i)=a
  d=matrix(d,nn,nn)
end
endfunction
