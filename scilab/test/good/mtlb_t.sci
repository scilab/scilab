function y=mtlb_t(a)
// Copyright INRIA
// Emulation function for Matlab transposition
// V.C.

  if type(a)==10 then
    tmp=mstr2sci(a)'
    y=[]
    for k=1:size(tmp,1)
      y(k)=strcat(tmp(k,:))
    end
  else
    y=a'
  end
endfunction

