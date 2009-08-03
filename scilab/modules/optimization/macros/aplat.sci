// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function [r,ind]=aplat(l,r)
//flattens a list. If l is constant it puts it in a list
//ind contains the list structure
  if type(l)<>15 then r=list(l);ind=-1;return;end
  n=size(l)
  [lhs,rhs]=argn(0)
  if rhs==1 then r=list(),nr=0,end
  ind=list()
  i=0
  nind=0
  for li=l
    i=i+1
    if type(li)==15 then 
      [r,ind1]=aplat(li,r)
      ni=size(ind1)
      for j=1:ni,nind=nind+1;ind(nind)=[i,ind1(j)];end
      nr=size(r)
    else
      nr=nr+1
      r(nr)=li
      nind=nind+1
      ind(nind)=i
    end
  end
endfunction
