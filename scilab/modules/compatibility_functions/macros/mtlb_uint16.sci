function i=mtlb_uint16(x)
// Copyright INRIA
// Emulation function for Matlab uint16()
// V.C.

if x==%inf then
  i=65535
else
  i=uint16(x)
end
endfunction
