function i=mtlb_uint32(x)
// Copyright INRIA
// Emulation function for Matlab uint32()
// V.C.

if x==%inf then
  i=4294967295
else
  i=uint32(x)
end
endfunction
