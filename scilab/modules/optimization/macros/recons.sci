// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function [r,ind]=recons(r,ind)
//reconstruct a list from a flat list (see aplat)
  if ind==-1 then return;end
  nr=size(r)
  ma=0
  for k=nr:-1:1
    mm=size(ind(k),'*');
    if ma<=mm then ma=mm;ki=k; end
  end

  if ma<=1 then return; end
  vi=ind(ki);vi=vi(1:ma-1);
  k=ki
  vj=vi

  while vj==vi
    k=k+1
    if k>nr then break; end
    vv=ind(k);
    if size(vv,'*')==ma then vj=vv(1:ma-1); else vj=[]; end
  end
  kj=k-1
  rt=list(r(ki))
  for k=ki+1:kj
    rt(k-ki+1)=r(ki+1)
    r(ki+1)=null()
    ind(ki+1)=null()
  end
  ind(ki)=vi
  r(ki)=rt
  [r,ind]=recons(r,ind)

endfunction
