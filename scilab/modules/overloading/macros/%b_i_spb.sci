// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function a=%b_i_spb(i,j,b,a)
// %spis(i,j,b,a) insert full matrix b into sparse matrix a for some special cases 
// a(i,j)=b
//!

[lhs,rhs]=argn(0)
if rhs==3 then
  a=b;
  b=j;
  [m,n]=size(a)
  a=a(:)
  a(i)=b
end
[ij,v]=spget(a)
if ij==[] then
  a=sparse([],[],[m,n])
else
  j=int((ij(:,1)-1)/m)+1
  i=ij(:,1)-m*(j-1)
  a=sparse([i j],v,[m,n])
end
endfunction
