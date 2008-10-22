// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [str]=pol2str(p)
//polynomial to character string
//
// p : polynomial (real)
// str : chracter string
//!
n=degree(p)
var=varn(p)
nv=length(var);while part(var,nv)==' ' then nv=nv-1,end;var=part(var,1:nv);

p=coeff(p)
if p(1)<>0 then
  if p(1)<0 then
    str=string(p(1))
  else
    str=' '+string(p(1))
  end
else
  if n==0 then
    str='0'
  else
    str=' '
  end
end
for k=1:n
  if p(k+1)<>0 then
    if p(k+1)<0 then
      str=str+'-'
    else
      str=str+'+'
    end
    if abs(p(k+1))<>1 then
      str=str+string(abs(p(k+1)))+'*'+var
    else
      str=str+var
    end
    if k>1 then str=str+'^'+string(k),end
  end
end
endfunction
