function x=mtlb_upper(y)
// Copyright INRIA
// Emulation function for upper() Matlab function
// V.C.

if type(y)==10 then
  x=convstr(y,"u");
else
  x=y
end
endfunction
