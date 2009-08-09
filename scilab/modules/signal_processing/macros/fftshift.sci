// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x = fftshift(x,job)
  if argn(2)<2 then job='all',end
  deff('sel=fun(sk)','c=ceil(sk/2);sel=[c+1:sk,1:c]')
  if job=='r' then job=1,elseif job=='c' then job=2,end
  ind=list()
  if job=='all' then
    for sk=size(x),ind($+1)=fun(sk),end
  else
    for sk=size(x),ind($+1)=:,end;
    ind(job)=fun(size(x,job))
  end
  x=x(ind(:))
endfunction
