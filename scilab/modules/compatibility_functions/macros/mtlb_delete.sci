function []=mtlb_delete(y)
// Copyright INRIA
// Emulation function for Matlab delete()
// V.C.

if type(y)==10 then 
  mdelete(y);
elseif type(y)==9 then 
  delete(y);
else
  error(msprintf(gettext("%s: This feature has not been implemented.\n"),"mtlb_delete"));
end

endfunction


