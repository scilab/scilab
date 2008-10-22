// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [x,ka,kb]=union(a,b)

// returns the union of  unique components of  vector a and b
[lhs,rhs]=argn()
if lhs==1 then
  [x,k]=unique([a(:);b(:)])
  x=x';k=k'
else
  na=size(a,'*')
  kab=[1:na,na+(1:size(b,'*'))]
  [x,k]=unique([a(:);b(:)])
  x=x'
  kab=kab(k)
  ka=kab(kab<na+1)
  kb=kab(kab>na)
  if kb<>[] then kb=kb-na,end
end
endfunction
