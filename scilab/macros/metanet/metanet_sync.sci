function [res]=metanet_sync(s)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs>1 then error(39), end
if rhs==0 then 
  s=-1
else
  // check s
  if s<>0&s<>1 then
    error('Argument must be 0 or 1')
  end
end
res=m6metasync(s)
