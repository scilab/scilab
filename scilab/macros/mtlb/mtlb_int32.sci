function i=mtlb_int32(x)
// Copyright INRIA
// Emulation function for Matlab int32()
// V.C.

if x==%inf then
  i=2147483647
elseif x==-%inf then
  i=-2147483648
elseif isnan(x) then
  i=0
else
  i=int32(x)
end
endfunction
