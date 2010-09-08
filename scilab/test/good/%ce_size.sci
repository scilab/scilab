// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function varargout=%ce_size(M,job)
// returns dimensions of a cell
[lhs,rhs]=argn(0)
dims=matrix(double(M.dims),1,-1);
if rhs==2 then
  if job=='*' then
    varargout(1)=prod(dims)
    return
  elseif type(job)==1 then
    if size(job,'*') >1 then error('Second argument is incorrect');end
    if job<=0|job>size(dims,'*') then 
      error('Second argument is incorrect'); 
    end
    varargout(1)=dims(job)
  else
    error('Second argument is incorrect')
  end
  return
end
if lhs==1 then
  varargout(1)=matrix(dims,1,-1)
else
  if lhs>size(dims,'*') then error('Too many LHS args'),end
  for k=1:lhs
    varargout(k)=dims(k)
  end
end
endfunction

