function i=mtlb_int16(x)
// Copyright INRIA
// Emulation function for Matlab int16()
// V.C.

if x==%inf then
  i=32767
elseif x==-%inf then
  i=-32768
elseif isnan(x) then
  i=0
else
  i=int16(x)
end
endfunction
