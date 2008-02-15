// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d=%c_triu(a,k)
// g_triu - implement triu function for sparse matrix, rationnal matrix ,..
[lhs,rhs]=argn(0)
if rhs==1 then k=0,end

[m,n]=size(a)
if k<=0 then
  mn=mini(m,n-k)
else
  mn=min(m+k,n)
end
a=matrix(a,m*n,1)
i=(1:mn)+((1:mn)+(k-1))*m
d=emptystr(m*n,1)
d(i)=a(i)
d=matrix(d,m,n)
endfunction
