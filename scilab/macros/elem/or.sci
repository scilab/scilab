function [x]=or(a,flag)
// or(a) returns the logical OR
// for scalar matrices, an entry is TRUE if it is not zero.
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then flag='*',end
if flag=='*' then
  select type(a)
  case 1 then
    k=find(abs(a)>0)
  case 4 then
    k=find(a)
  case 6 then
    k=find(a)
  else
    error('argument must be a boolean or a real matrix!')
  end
  x=k<>[]
else
  a=bool2s(a)
  x=sum(a,flag)>=1
end

