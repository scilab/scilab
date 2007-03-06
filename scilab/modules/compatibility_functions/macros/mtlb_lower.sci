function x=mtlb_lower(y)
// Copyright INRIA
// Emulation function for lower() Matlab function
// V.C.
if type(y)==10 then
  x=convstr(y,"l");
else
  x=y
end
endfunction
