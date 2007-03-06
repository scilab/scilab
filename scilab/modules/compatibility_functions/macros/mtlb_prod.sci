function r=mtlb_prod(a,dim)
// Copyright INRIA
// Emulation function for prod() Matlab function
// V.C.

rhs=argn(2)

if rhs==1 then
  // Scalar, vector or empty matrix
  if find(size(a)==1)<>[] | find(size(a)==0)<>[] then
    r=prod(a)
  else
    r=prod(a,1)
  end
else
  if dim<=size(size(a),"*") then
    r=prod(a,dim)
  else
    r=a
  end
end
endfunction


