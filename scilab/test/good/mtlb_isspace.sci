function y=mtlb_isspace(x)
// Copyright INRIA
// Emulation function for isspace() Matlab function
// V.C.

if type(x)==10 then
  y=asciimat(x)==32
else
  y=zeros(x)
end
endfunction
