// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%p_x_r(p,r)
// r=%p_x_r(p,r)  <=> r=p.*r   polynomial.* rational
//author Serge Steer, INRIA
//!

  r.num=p.*r.num
  sz=size(r.num)
  if size(sz,'*')<=2 then
    r=simp(r)
  else
    [num,den]=simp(r.num.entries,r.den.entries)
    r.num=matrix(num,sz)
    r.den=matrix(den,sz)
  end
endfunction
