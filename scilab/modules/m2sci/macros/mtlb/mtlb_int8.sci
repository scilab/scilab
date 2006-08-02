function i=mtlb_int8(x)
// Copyright INRIA
// Emulation function for Matlab int8()
// V.C.

if x==%inf then
  i=127
elseif x==-%inf then
  i=-128
else
  i=int8(x)
end
endfunction
