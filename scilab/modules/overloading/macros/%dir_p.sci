// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function %dir_p(L)
  ll= lines();ll=max(ll(1),20)
  name=L.name;
  if name==[] then mprintf("%s \n","[]"); return,end
  
  // If it is a directory, add a trailing "/"
  tmp = find(L.isdir);
  name(tmp)=name(tmp)+filesep();

  nc=6
  l1=length(name)
  while %t
    l2=l1;
    n=ceil(size(l1,'*')/nc)
    if n*nc>size(name,'*') then l2(n*nc)=0;l1(n*nc)=0;end
    l=max(matrix(l2,-1,nc),'r');
    if sum(l)<ll|nc==1 then break,end
    nc=nc-1;
  end
  if n*nc>size(name,'*') then name(n*nc)='';end
  name=matrix(name,-1,nc)
  txt=emptystr(n,1)
  for k=1:nc
    txt=txt+part(name(:,k),1:l(k)+1)
  end
  mprintf("%s \n",txt)
  
endfunction

