function B=mtlb_cumsum(A,dim)
// Copyright INRIA
// Emulation function for Matlab cumsum()
// V.C.

[lhs,rhs]=argn()

if rhs==1 then
  if size(a,1)==1|size(a,2)==1 then
    r=cumsum(a)
  else
    r=cumsum(a,firstnonsingleton(A))
  end
else
  if dim<=size(size(A),"*") then
    B=cumsum(A,dim)
  else
    B=A
  end
end
endfunction
