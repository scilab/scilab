function [w]=mtlb_conv(u,v)
// Copyright INRIA
// Emulation function for Matlab conv()
// V.C.

w=clean(convol(u,v))
if size(u,1)>1 | size(v,1)>1 then
  w=w.'
end
endfunction


