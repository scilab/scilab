// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=mtlb_box(axes_handle,val)
// Emulation function for Matlab box()

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
  error(msprintf(gettext("%s: This feature has not been implemented.\n"),"mtlb_box"));
end

endfunction
