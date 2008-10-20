function mtlb_hold(flag)
// Copyright INRIA
// Emulation function for Matlab hold()
// V.C.

rhs=argn(2)
if rhs<=0 then
  a=gca();
  if a.auto_clear=="off" then
    a.auto_clear="on"
  else
    a.auto_clear="off"
  end
else
  if flag=="on" then
    a=gca();a.auto_clear="off"
  elseif flag=="off" then
    a=gca();a.auto_clear="on"
  else
    error("Not implemented")
  end
end
endfunction
