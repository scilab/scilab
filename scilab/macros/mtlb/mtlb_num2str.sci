function [str]=mtlb_num2str(x,f)
// Copyright INRIA
// Emulation function for Matlab num2str()
// V.C.

rhs=argn(2)

if rhs==1 then // num2str(x)
  if isempty(x) then
    str=''
  else
    str=string(x)
  end
else
  if type(f)==10 then
    s=msprintf(f,x)
  else
    error("Not implemented")
  end
end
endfunction


