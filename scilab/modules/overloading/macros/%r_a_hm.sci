// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=%r_a_hm(f,m)
//f = f+m, 
//f: transfer matrix, m : hypermatrix
  //author Serge Steer INRIA
//!
  [num,den]=f(['num','den'])
  szf=size(den)
  szm=size(m)

  if and(szf>=0)&and(szm>=0) then
    num=num(:);den=den(:);m=m(:)
    if prod(szf)==1&prod(szm)>1 then
      den=den(ones(m))
    end
    [num,den]=simp(num+m.*den,den)
    num=matrix(num,szf)
    den=matrix(den,szf)
    f=rlist(num,den,f.dt)
  else
    error(8)
  end
endfunction
