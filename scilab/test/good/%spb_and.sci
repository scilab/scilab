function x=%spb_and(a,flag)
// or(a) returns the logical OR
// for scalar matrices, an entry is TRUE if it is not zero.
//!
// Copyright INRIA
if argn(2)==1 then flag='*',end
if flag=='*' then
  x=find(~a,1)==[]
 else
  x=sum(bool2s(a),flag)==size(a,flag)
end
endfunction
