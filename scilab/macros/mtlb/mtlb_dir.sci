function []=mtlb_dir(path)
// Copyright INRIA
// Emulation function for dir() Matlab function
// V.C.
rhs=argn(2)
if rhs<=0 then
  if MSDOS then
    unix_w("dir");
  else
    unix_w("ls");
  end
else
  if MSDOS then
    unix_w("dir "+path);
  else
    unix_w("ls "+path);
  end
end

endfunction
