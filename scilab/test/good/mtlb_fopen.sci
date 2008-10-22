function [fid,mess]=mtlb_fopen(filename,permission)
// Copyright INRIA
// Emulation function for fopen() Matlab function
// V.C.

[lhs,rhs]=argn()

[fid,err]=mopen(filename,permission,0)
if err<0 then
  fid=-1
  mess="Cannot open file!"
else
  mess=""
end

endfunction

