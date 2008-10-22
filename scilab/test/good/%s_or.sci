function x=%s_or(a,flag)
// or(a) returns the logical OR
// for scalar matrices, an entry is TRUE if it is not zero.
//!
// Copyright INRIA
if argn(2)==1 then flag='*',end
if flag=='*' then
  x=find(abs(a)>0,1)<>[]
else
  if a==[] then x=[],return,end
  x=sum(bool2s(a),flag)>=1
end
endfunction
