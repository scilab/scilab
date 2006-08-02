function r=mtlb_false(a)
// Copyright INRIA
// Emulation function for false() Matlab function
// V.C.

if and(size(a)==[1 1]) then
  r=zeros(a,a)
else
  tmp=list()
  for k=1:size(a,"*")
    tmp(k)=a(k)
  end
  r=zeros(tmp(1:$))
end
endfunction
