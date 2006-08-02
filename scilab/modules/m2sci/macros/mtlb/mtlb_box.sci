function []=mtlb_box(axes_handle,val)
// Copyright INRIA
// Emulation function for Matlab box()
// V.C.

rhs=argn(2)

a=gca();

if rhs<=0 then // box toggle
  if a.box=="on" then
    a.box="off"
  else
    a.box="on"
  end
elseif rhs==1 then  
  if type(axes_handle)==10 then // box on/off
    a.box=convstr(axes_handle,"l")
  else // box(axes_handle)
    if axes_handle.box=="on" then
      axes_handle.box="off"
    else
      axes_handle.box="on"
    end
  end
elseif rhs==2 then // box(axes_handle,...)
  axes_handle.box=convstr(val,"l")
else
  error("Not yet implemented");
end

endfunction
