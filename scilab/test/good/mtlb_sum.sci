function r=mtlb_sum(a,dim)
// Copyright INRIA
// Emulation function for sum() Matlab function
// V.C.

rhs=argn(2)

if rhs==1 then
  // Scalar, vector or empty matrix
  if find(size(a)==1)<>[] | find(size(a)==0)<>[] then
    r=sum(a)
  else
    r=sum(a,1)
  end
else
  if dim<=size(size(a),"*") then
    r=sum(a,dim)
  else
    r=a
  end
end
endfunction

