function R=mtlb_rem(X,Y)
// Copyright INRIA
// Emulation function for Matlab rem()
// V.C.

if Y==0 then
  R = %nan;
else
  R = X-fix(X./Y).*Y;
end

endfunction
