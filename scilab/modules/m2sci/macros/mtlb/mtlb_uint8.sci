function i=mtlb_uint8(x)
// Copyright INRIA
// Emulation function for Matlab uint8()
// V.C.

if x==%inf then
  i=255
else
  i=uint8(x)
end
endfunction
