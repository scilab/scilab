function B=mtlb_cumprod(A,dim)
// Copyright INRIA
// Emulation function for Matlab cumprod()
// V.C.

[lhs,rhs]=argn()

if rhs==1 then
  if size(a,1)==1|size(a,2)==1 then
    r=cumprod(a)
  else
    r=cumprod(a,firstnonsingleton(A))
  end
else
  if dim<=size(size(A),"*") then
    B=cumprod(A,dim)
  else
    B=A
  end
end
endfunction
