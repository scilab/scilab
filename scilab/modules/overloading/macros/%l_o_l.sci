// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [r]=%l_o_l(l1,l2)
//l1==l2
//!

n1=length(l1)


r=n1==length(l2)
if r&n1>0 then
  r=%f(ones(1,n1))
  sel=%f(ones(1,n1))
  k1=definedfields(l1)
  k2=definedfields(l2)
  for i=intersect(k1,k2),
    if and(getfield(i,l1)==getfield(i,l2)) then r(i)= %t,end
  end
  s1=1:n1;s1(k1)=[];
  s2=1:n1;s2(k2)=[];
  k1=intersect(s1,s2)
  if k1<>[] then r(intersect(s1,s2))=%t,end
end
endfunction
