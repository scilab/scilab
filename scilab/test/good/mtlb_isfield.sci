function [tf]=mtlb_isfield(st,f)
// Copyright INRIA
// Emulation function for dir() Matlab function
// V.C.

if typeof(st)<>"st" then
  tf=%f;
  return
end

allf=getfield(1,st);
tf=or(allf(3:$)==f);
endfunction
