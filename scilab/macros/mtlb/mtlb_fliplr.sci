function x=mtlb_fliplr(y)
// Copyright INRIA
// Emulation function for fliplr() Matlab function
// V.C.

if type(y)==1 then 
  x=y(:,$:-1:1)
elseif type(y)==10 then 
  tmp = asciimat(y)
  tmp=tmp(:,$:-1:1)
  x=[];
  for k=1:size(tmp,1)
    x=[x;ascii(tmp(k,:))]
  end
elseif type(y)==4 then 
  tmp = bool2s(y) 
  x=tmp(:,$:-1:1)
end
endfunction


