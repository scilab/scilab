function []=mtlb_delete(y)
// Copyright INRIA
// Emulation function for Matlab delete()
// V.C.

if type(y)==10 then 
  mdelete(y);
elseif type(y)==9 then 
  delete(y);
else
  error(gettext("errors","compatibility_functions_error_2"));
end

endfunction


