// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [str]=mtlb_num2str(x,f)
// Emulation function for Matlab num2str()

rhs=argn(2)

if rhs==1 then // num2str(x)
  if isempty(x) then
    str=''
  else
    str=string(x)
  end
else
  if type(f)==10 then
    s=msprintf(f,x)
  else
    error(msprintf(gettext("%s: Not yet implemented.\n"),"mtlb_num2str"))
  end
end
endfunction


