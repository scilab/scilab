// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s1=%r_i_r(i,j,s1,s2)
// %r_i_r(i,j,s1,s2) insertion  s2(i,j)=s1
//!

[lhs,rhs]=argn(0)
if rhs==3 then
  [s1,s2]=sysconv(j,s1)
  [n1,n2]=size(s2('den'))
  n=s2('num');n(i)=s1('num'),
  d=ones(n);d(1:n1,1:n2)=s2('den');d(i)=s1('den')
else
  [s1,s2]=sysconv(s1,s2)
  [n1,n2]=size(s2('den'))
  n=s2('num');n(i,j)=s1('num'),
  d=ones(n);d(1:n1,1:n2)=s2('den');d(i,j)=s1('den')
end
s1=rlist(n,d,s1('dt'))
endfunction
