function r=mtlb_rand(a)
// Copyright INRIA
// Emulation function for rand() Matlab function
// V.C.

if and(size(a)==[1 1]) then
  r=rand(a,a)
else
  tmp=list()
  for k=1:size(a,"*")
    tmp(k)=a(k)
  end
  r=rand(tmp(1:$))
end
endfunction
