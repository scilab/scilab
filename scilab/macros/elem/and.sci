function [x]=and(a,flag)
// Logical and. A is a matrix of booleans or a real matrix.
// Returns True or non-zero entries.
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then flag='*',end
if flag=='*' then
  select type(a)
  case 1 then
    k=find(abs(a)==0)
  case 4 then
    k=find(a==%f)
  case 6 then
    k=find(a)
    if size(k,'*')==size(a,'*') then x=%t,return,end
  else
    error('input must be boolean or real matrix!')
  end
  x=k==[]
else
  a=bool2s(a)
  x=sum(a,flag)==size(a,flag)
end
