function x=mtlb_full(y)
// Copyright INRIA
// Emulation function for full() Matlab function
// V.C.
if type(y)==1 then 
  x=full(y)
elseif type(y)==10 then 
  tmp = asciimat(y)
  tmp=full(tmp)
  x=[];
  for k=1:size(tmp,1)
    x=[x;ascii(tmp(k,:))]
  end
elseif type(y)==4 then 
  x=full(bool2s(y))
end
endfunction


