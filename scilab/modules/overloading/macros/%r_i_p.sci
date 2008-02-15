// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f2=%r_i_p(i,j,f2,n)
// f2=%r_i_p(i,j,f2,p)   insertion
//

[lhs,rhs]=argn(0)
if rhs==3 then
  n=f2;f2=j
  d=ones(n);
  n(i)=f2('num'),d(i)=f2('den')
else
  d=ones(n);
  n(i,j)=f2('num'),d(i,j)=f2('den')
end
f2=rlist(n,d,f2('dt'))
endfunction
