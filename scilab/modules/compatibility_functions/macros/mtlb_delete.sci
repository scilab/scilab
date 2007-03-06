function []=mtlb_delete(y)
// Copyright INRIA
// Emulation function for Matlab delete()
// V.C.

if type(y)==10 then 
  mdelete(y);
elseif type(y)==9 then 
  delete(y);
else
  error("Not yet implemented");
end

endfunction


