function r=mtlb_eye(a)
// Copyright INRIA
// Emulation function for eye() Matlab function
// V.C.

if and(size(a)==[1 1]) then
  r=eye(a,a)
else
  r=eye(a(1),a(2))
end
endfunction
