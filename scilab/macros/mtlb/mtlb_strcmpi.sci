function y=mtlb_strcmpi(A,B)
// Copyright INRIA
// Emulation function for strcmpi() Matlab function
// V.C.

if type(A)==10 & type(B)==10 then
  y=convstr(A)==convstr(B)
else
  y=%F
end
endfunction
