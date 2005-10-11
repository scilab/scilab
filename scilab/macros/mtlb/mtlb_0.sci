function y=mtlb_0(a)
// Copyright INRIA
// Emulation function for Matlab dot-transposition
// V.C.

  if type(a)==10 then
    tmp=asciimat(a).'
    y=[]
    for k=1:size(tmp,1)
      y(k)=strcat(ascii(tmp(k,:)))
    end
  elseif or(type(a)==[4,6]) then
    y=bool2s(a).'
  else
    y=a.'
  end

endfunction
