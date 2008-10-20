function r=mtlb_randn(a,opt)
// Copyright INRIA
// Emulation function for randn() Matlab function
// V.C.

[lhs,rhs]=argn()
if rhs==1 then
  if and(size(a)==[1 1]) then
    r=rand(a,a,"normal")
  else
    tmp=list()
    for k=1:size(a,"*")
      tmp(k)=a(k)
    end
    tmp($+1)="normal"
    r=rand(tmp(1:$))
  end
else
  if and(size(a)==[1 1]) then
    r=randn(a,a,opt)
  else
    tmp=list()
    for k=1:size(a,"*")
      tmp(k)=a(k)
    end
    tmp($+1)="normal"
    r=rand(tmp(1:$))
  end
end
endfunction
