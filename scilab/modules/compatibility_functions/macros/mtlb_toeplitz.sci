function z=mtlb_toeplitz(x,y)
// Copyright INRIA
// Emulation function for Matlab toeplitz()
// V.C.

if argn(2)==1 then
  if isreal(x) then
    if find(size(x)==1)==[] then
      tmp=toeplitz(x);
      s=size(x)
      z=tmp(1:s($),:)
    else
      z=toeplitz(x);
    end
    return
  end
  y=x
  x=conj(x)
  x(1)=conj(x(1))
end
if x(1)<>y(1) then
  y(1)=x(1)
end
if find(size(x)==1)==[] then
  tmp=toeplitz(x,y);
  s=size(x)
  z=tmp(1:s($),:)
else
  z=toeplitz(x,y);
end

endfunction
