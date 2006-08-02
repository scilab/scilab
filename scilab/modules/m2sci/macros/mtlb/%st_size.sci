function varargout=%st_size(M,job)
// Copyright INRIA
// returns dimensions of an hyper matrix
[lhs,rhs]=argn(0)
dims=matrix(double(M.dims),1,-1)
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
