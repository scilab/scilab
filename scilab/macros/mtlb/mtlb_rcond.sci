function c=mtlb_rcond(a)
// Copyright INRIA
// Emulation function for rcond() Matlab function
// V.C.

if isempty(a) then
  c=%inf
else
  c=rcond(a)
end
endfunction
