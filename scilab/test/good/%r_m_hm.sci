// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f1=%r_m_hm(f1,n2)
// %r_m_s(f1,n2)  
//f1 =  f1*n2
//author Serge Steer INRIA
//!

  [n1,d1]=f1(['num','den']),
  sz1=size(n1);
  if prod(sz1)==0 then return,end
  sz2=size(n2);
  n2=n2(:)

  if or(sz1==-1) then 
    n1=n1+0;d1=d1+0;
  end
  if prod(sz1)==1 then
    num=n1*n2,
    den=d1(ones(n2))
  else,
    error(10)
  end,

  f1=rlist(matrix(num,sz2),matrix(den,sz2),f1.dt)
endfunction

