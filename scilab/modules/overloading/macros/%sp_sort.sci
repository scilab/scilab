// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function A=%sp_sort(A,flag)

[ij,v,mn]=spget(A);
if mn(2)==1 then
  last=find(v<0);
  first=find(v>0);
  nn=size(v,'*');
  v([1:size(first,'*'),nn-size(last,'*')+1:nn])=[sort(v(first));sort(v(last))];
  A=sparse(ij,v,mn);
elseif mn(1)==1 then
  last=find(v<0);
  first=find(v>0);
  nn=size(v,'*');
  v([1:size(first,'*'),nn-size(last,'*')+1:nn])=[sort(v(first));sort(v(last))];
  A=sparse(ij,v,mn);  
else
  error('Sorry: Only sparse VECTORS can be sorted!');
end
endfunction
